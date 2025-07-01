#include <arpa/inet.h> /* inet_ntoa */
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define LISTENQ 1024 /* second argument to listen() */
#define MAXLINE 1024 /* max length of a line */
#define RIO_BUFSIZE 1024

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 9999 /* use this port if none given as arg to main() */
#endif

#ifndef FORK_COUNT
#define FORK_COUNT 10
#endif

#ifndef NO_LOG_ACCESS
#define LOG_ACCESS
#endif

typedef struct {
  int rio_fd;                /* descriptor for this buf */
  int rio_cnt;               /* unread byte in this buf */
  char *rio_bufptr;          /* next unread byte in this buf */
  char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct sockaddr SA;

typedef struct {
  char filename[512];
  off_t offset; /* for support Range */
  size_t end;
} http_request;

typedef struct {
  const char *extension;
  const char *mime_type;
} mime_map;

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types

mime_map meme_types[] = {
    {".aac", "audio/aac"},
    {".abw", "application/x-abiword"},
    {".arc", "application/x-freearc"},
    {".avi", "video/x-msvideo"},
    {".azw", "application/vnd.amazon.ebook"},
    {".bin", "application/octet-stream"},
    {".bmp", "image/bmp"},
    {".bz", "application/x-bzip"},
    {".bz2", "application/x-bzip2"},
    {".csh", "application/x-csh"},
    {".css", "text/css"},
    {".csv", "text/csv"},
    {".doc", "application/msword"},
    {".docx",
     "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".eot", "application/vnd.ms-fontobject"},
    {".epub", "application/epub+zip"},
    {".gz", "application/gzip"},
    {".gif", "image/gif"},
    {".htm", "text/html"},
    {".html", "text/html"},
    {".ico", "image/vnd.microsoft.icon"},
    {".ics", "text/calendar"},
    {".jar", "application/java-archive"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".js", "text/javascript"},
    {".json", "application/json"},
    {".jsonld", "application/ld+json"},
    {".mid", "audio/midi audio/x-midi"},
    {".midi", "audio/midi audio/x-midi"},
    {".mjs", "text/javascript"},
    {".mp3", "audio/mpeg"},
    {".mp4", "video/mp4"},
    {".mpeg", "video/mpeg"},
    {".mpkg", "application/vnd.apple.installer+xml"},
    {".odp", "application/vnd.oasis.opendocument.presentation"},
    {".ods", "application/vnd.oasis.opendocument.spreadsheet"},
    {".odt", "application/vnd.oasis.opendocument.text"},
    {".oga", "audio/ogg"},
    {".ogv", "video/ogg"},
    {".ogx", "application/ogg"},
    {".opus", "audio/opus"},
    {".otf", "font/otf"},
    {".png", "image/png"},
    {".pdf", "application/pdf"},
    {".php", "application/x-httpd-php"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".pptx",
     "application/"
     "vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".rar", "application/vnd.rar"},
    {".rtf", "application/rtf"},
    {".sh", "application/x-sh"},
    {".svg", "image/svg+xml"},
    {".swf", "application/x-shockwave-flash"},
    {".tar", "application/x-tar"},
    {".tif", "image/tiff"},
    {".tiff", "image/tiff"},
    {".ts", "video/mp2t"},
    {".ttf", "font/ttf"},
    {".txt", "text/plain"},
    {".vsd", "application/vnd.visio"},
    {".wav", "audio/wav"},
    {".weba", "audio/webm"},
    {".webm", "video/webm"},
    {".webp", "image/webp"},
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".xhtml", "application/xhtml+xml"},
    {".xls", "application/vnd.ms-excel"},
    {".xlsx",
     "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xml", "text/xml"},
    {".xul", "application/vnd.mozilla.xul+xml"},
    {".zip", "application/zip"},
    {".3gp", "video/3gpp"},
    {".3g2", "video/3gpp2"},
    {".7z", "application/x-7z-compressed"},
    {NULL, NULL},
};

char *default_mime_type = "text/plain";

// Add these function prototypes
void client_error(int fd, int status, char *msg, char *longmsg,
                  const char *redirect_url);
int check_header(const char *submitted_flag);
void serve_static(int out_fd, int in_fd, http_request *req, size_t total_size);

void rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;
  rp->rio_cnt = 0;
  rp->rio_bufptr = rp->rio_buf;
}

ssize_t writen(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR) { /* interrupted by sig handler return */
        nwritten = 0;       /* and call write() again */
      } else {
        return -1; /* errorno set by write() */
      }
    }
    nleft -= nwritten;
    bufp += nwritten;
  }
  return n;
}

/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
  int cnt;
  while (rp->rio_cnt <= 0) { /* refill if buf is empty */

    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
    if (rp->rio_cnt < 0) {
      if (errno != EINTR) { /* interrupted by sig handler return */
        return -1;
      }
    } else if (rp->rio_cnt == 0) { /* EOF */
      return 0;
    } else
      rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr */
  }

  /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
  cnt = n;
  if (rp->rio_cnt < n) {
    cnt = rp->rio_cnt;
  }
  memcpy(usrbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;
  return cnt;
}

/*
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  int n, rc;
  char c, *bufp = usrbuf;

  for (n = 1; n < maxlen; n++) {
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      *bufp++ = c;
      if (c == '\n') {
        break;
      }
    } else if (rc == 0) {
      if (n == 1) {
        return 0; /* EOF, no data read */
      } else {
        break; /* EOF, some data was read */
      }
    } else {
      return -1; /* error */
    }
  }
  *bufp = 0;
  return n;
}

void format_size(char *buf, struct stat *stat) {
  if (S_ISDIR(stat->st_mode)) {
    sprintf(buf, "%s", "[DIR]");
  } else {
    off_t size = stat->st_size;
    if (size < 1024) {
      sprintf(buf, "%lu", size);
    } else if (size < 1024 * 1024) {
      sprintf(buf, "%.1fK", (double)size / 1024);
    } else if (size < 1024 * 1024 * 1024) {
      sprintf(buf, "%.1fM", (double)size / 1024 / 1024);
    } else {
      sprintf(buf, "%.1fG", (double)size / 1024 / 1024 / 1024);
    }
  }
}

void handle_directory_request(int out_fd, int dir_fd, char *filename) {
  char buf[MAXLINE], m_time[32], size[16];
  struct stat statbuf;
  sprintf(buf, "HTTP/1.1 200 OK\r\n%s%s%s%s%s",
          "Content-Type: text/html\r\n\r\n", "<html><head><style>",
          "body{font-family: monospace; font-size: 13px;}",
          "td {padding: 1.5px 6px;}", "</style></head><body><table>\n");
  writen(out_fd, buf, strlen(buf));
  DIR *d = fdopendir(dir_fd);
  struct dirent *dp;
  int ffd;
  while ((dp = readdir(d)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
      continue;
    }
    if ((ffd = openat(dir_fd, dp->d_name, O_RDONLY)) == -1) {
      perror(dp->d_name);
      continue;
    }
    fstat(ffd, &statbuf);
    strftime(m_time, sizeof(m_time), "%Y-%m-%d %H:%M",
             localtime(&statbuf.st_mtime));
    format_size(size, &statbuf);
    if (S_ISREG(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) {
      char *d = S_ISDIR(statbuf.st_mode) ? "/" : "";
      sprintf(
          buf,
          "<tr><td><a href=\"%s%s\">%s%s</a></td><td>%s</td><td>%s</td></tr>\n",
          dp->d_name, d, dp->d_name, d, m_time, size);
      writen(out_fd, buf, strlen(buf));
    }
    close(ffd);
  }
  sprintf(buf, "</table></body></html>");
  writen(out_fd, buf, strlen(buf));
  closedir(d);
}

static const char *get_mime_type(char *filename) {
  char *dot = strrchr(filename, '.');
  if (dot) {  // strrchar Locate last occurrence of character in string
    mime_map *map = meme_types;
    while (map->extension) {
      if (strcmp(map->extension, dot) == 0) {
        return map->mime_type;
      }
      map++;
    }
  }
  return default_mime_type;
}

int open_listenfd(int port) {
  int listenfd, optval = 1;
  struct sockaddr_in serveraddr;

  /* Create a socket descriptor */
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  /* Eliminates "Address already in use" error from bind. */
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
                 sizeof(int)) < 0) {
    return -1;
  }

  // 6 is TCP's protocol number
  // enable this, much faster : 4000 req/s -> 17000 req/s
  if (setsockopt(listenfd, 6, TCP_CORK, (const void *)&optval, sizeof(int)) <
      0) {
    return -1;
  }

  /* Listenfd will be an endpoint for all requests to port
     on any IP address for this host */
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port);
  if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) {
    return -1;
  }

  /* Make it a listening socket ready to accept connection requests */
  if (listen(listenfd, LISTENQ) < 0) {
    return -1;
  }
  return listenfd;
}

void url_decode(char *src, char *dest, int max) {
  char *p = src;
  char code[3] = {0};
  while (*p && --max) {
    if (*p == '%') {
      memcpy(code, ++p, 2);
      *dest++ = (char)strtoul(code, NULL, 16);
      p += 2;
    } else {
      *dest++ = *p++;
    }
  }
  *dest = '\0';
}

int check_header(const char *submitted_flag) {
  unsigned char encrypted_flag[] = {
      0x33, 0x20, 0x38, 0x58, 0x12, 0x28, 0x5C, 0x47, 0x29, 0x52, 0x2D, 0x0F,
      0x5A, 0x0A, 0x0E, 0x00, 0x0F, 0x58, 0x13, 0x50, 0x19, 0x5A, 0x19, 0x34,
      0x58, 0x31, 0x33, 0x43, 0x13, 0x41, 0x04, 0x5A, 0x19, 0x34, 0x58, 0x2C,
      0x33, 0x53, 0x46, 0x03, 0x1E, 0x48, 0x4A, 0x4A, 0x14, 0x00};
  const size_t flag_size =
      sizeof(encrypted_flag) - 1;  // -1 for null terminator
  // apparently this "handcrafted" obfuscation won't work due to compiler optimization XD
  char key[11];
  key[9] = '3';
  key[3] = 'k';
  key[1] = 'i';
  key[5] = '_';
  key[8] = 'v';
  key[6] = 'l';
  key[7] = '0';
  key[0] = 'r';
  key[2] = 'k';
  key[4] = 'i';
  key[10] = '\0';

  // Decrypt the flag
  for (size_t i = 0; i < flag_size; i++) {
    encrypted_flag[i] ^= key[i % (sizeof(key) - 1)];
  }

  // Compare byte by byte
  const unsigned char *s1 = encrypted_flag;
  const unsigned char *s2 = (const unsigned char *)submitted_flag;

  for (size_t i = 0; i < flag_size; i++) {
    if (s2[i] == '\0' || s1[i] != s2[i]) {
      return 0;
    }
  }

  // Make sure submitted_flag isn't longer
  return s2[flag_size] == '\0';
}

void parse_request(int fd, http_request *req) {
  rio_t rio;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE];
  req->offset = 0;
  req->end = 0; /* default */

  rio_readinitb(&rio, fd);
  rio_readlineb(&rio, buf, MAXLINE);
  sscanf(buf, "%s %s", method, uri); /* version is not cared */
  // printf("uri = %s",uri);
  /* read all */
  while (buf[0] != '\n' && buf[1] != '\n') { /* \n || \r\n */
    rio_readlineb(&rio, buf, MAXLINE);
    if (buf[0] == 'R' && buf[1] == 'a' && buf[2] == 'n') {
      sscanf(buf, "Range: bytes=%ld-%u", &req->offset, &req->end);
      if (req->end != 0) {
        req->end++;
      }
    }
    // Check for AIS3-Flag header
    if (strncmp(buf, "AIS3-Flag:", 9) == 0) {
      char *flag_start = buf + 9;
      // Skip any whitespace and colons after "AIS3-Flag:"
      while (*flag_start == ' ' || *flag_start == ':') {
        flag_start++;
      }
      // Remove trailing \r\n if present
      char *newline = strchr(flag_start, '\r');
      if (newline) *newline = 0;
      newline = strchr(flag_start, '\n');
      if (newline) *newline = 0;

      if (check_header(flag_start)) {
        client_error(fd, 200, "Flag Correct!",
                     "Congratulations! You found the correct flag!", NULL);
        close(fd);
        return;
      } else {
        client_error(fd, 403, "Wrong Flag",
                     "Sorry, that's not the correct flag. Try again!", NULL);
        close(fd);
        return;
      }
    }
  }

  char *filename = uri;
  if (uri[0] == '/') {
    filename = uri + 1;
    int length = strlen(filename);
    if (length == 0) {
      filename = ".";
    } else {
      int i = 0;
      for (; i < length; ++i) {
        if (filename[i] == '?') {
          filename[i] = '\0';
          break;
        }
      }
    }
  }
  url_decode(filename, req->filename, MAXLINE);
}

#ifdef LOG_ACCESS
void log_access(int status, struct sockaddr_in *c_addr, http_request *req) {
  printf("%s:%d %d - '%s' (%s)\n", inet_ntoa(c_addr->sin_addr),
         ntohs(c_addr->sin_port), status, req->filename,
         get_mime_type(req->filename));
}
#endif

void client_error(int fd, int status, char *msg, char *longmsg,
                  const char *redirect_url) {
  char buf[MAXLINE];

  if (redirect_url) {
    sprintf(buf, "HTTP/1.1 %d %s\r\n", status, msg);
    sprintf(buf + strlen(buf), "Location: %s\r\n", redirect_url);
    sprintf(buf + strlen(buf), "Content-length: 0\r\n\r\n");
  } else {
    sprintf(buf, "HTTP/1.1 %d %s\r\n", status, msg);
    sprintf(buf + strlen(buf), "Content-length: %zu\r\n\r\n", strlen(longmsg));
    sprintf(buf + strlen(buf), "%s", longmsg);
  }

  writen(fd, buf, strlen(buf));
}

void serve_static(int out_fd, int in_fd, http_request *req,
                  size_t total_size) {  // total size is useless XDD
  char buf[256];
  if (req->offset > 0) {
    sprintf(buf, "HTTP/1.1 206 Partial\r\n");
    sprintf(buf + strlen(buf), "Content-Range: bytes %ld-%u/%u\r\n",
            req->offset, req->end, total_size);
  } else {
    sprintf(buf, "HTTP/1.1 200 OK\r\nAccept-Ranges: bytes\r\n");
  }
  sprintf(buf + strlen(buf), "Cache-Control: no-cache\r\n");
  // sprintf(buf + strlen(buf), "Cache-Control: public,
  // max-age=315360000\r\nExpires: Thu, 31 Dec 2037 23:55:55 GMT\r\n");

  sprintf(buf + strlen(buf), "Content-length: %u\r\n",
          (unsigned int)(req->end - req->offset));
  sprintf(buf + strlen(buf), "Content-type: %s\r\n\r\n",
          get_mime_type(req->filename));

  writen(out_fd, buf, strlen(buf));
  off_t offset = req->offset; /* copy */
  char buffer[4096];
  ssize_t bytes_read;

  // Seek to the requested offset
  if (lseek(in_fd, offset, SEEK_SET) < 0) {
    return;
  }

  // Read and write until we've sent all requested bytes
  while (offset < req->end &&
         (bytes_read = read(in_fd, buffer, sizeof(buffer))) > 0) {
    // Don't write more than the client requested
    if (offset + bytes_read > req->end) {
      bytes_read = req->end - offset;
    }

    if (writen(out_fd, buffer, bytes_read) < 0) {
      break;
    }
    offset += bytes_read;
  }
  close(out_fd);
}

void process(int fd, struct sockaddr_in *clientaddr) {
#ifdef LOG_ACCESS
  printf("accept request, fd is %d, pid is %d\n", fd, getpid());
#endif

  http_request req;
  parse_request(fd, &req);

  struct stat sbuf;
  int status = 200, ffd = open(req.filename, O_RDONLY);
  // printf("filename: %s\n",req.filename);
  if (ffd <= 0) {
    status = 404;
    char *msg = "Not Found";
    client_error(fd, status, msg, "",
                 "https://www.youtube.com/watch?v=dQw4w9WgXcQ");
  } else {
    fstat(ffd, &sbuf);
    // printf("mode: %d ffd: %d\n",sbuf.st_mode,ffd);
    // Due to some unknown error, my wsl is not able to serve the file
    // But I copied the same code to the code and it worked??? WTF...
    if (S_ISREG(sbuf.st_mode)) {
      if (req.end == 0) {
        req.end = sbuf.st_size;
      }
      if (req.offset > 0) {
        status = 206;
      }
      printf("serving file size %ld\n", (long)sbuf.st_size);
      serve_static(fd, ffd, &req, sbuf.st_size);
    } else if (S_ISDIR(sbuf.st_mode)) {
      status = 200;
      handle_directory_request(fd, ffd, req.filename);
    } else {
      status = 400;
      char *msg = "Unknow Error";
      client_error(fd, status, "Error", msg, NULL);
    }
    close(ffd);
  }
#ifdef LOG_ACCESS
  log_access(status, clientaddr, &req);
#endif
}

void print_help() {
  printf("AIS3 TINY WEBSERVER\n");
  printf(
      "Forked from https://github.com/7890/tiny-web-server\nAdapted by "
      "pwn2ooown with lots of love <3\n");
  printf("Usage: tiny [dir] [port]\n");
  printf("tiny            #use default port, serve current dir\n");
  printf("tiny /tmp       #use default port, serve given dir\n");
  printf("tiny 1234       #use given port, serve current dir\n");
  printf("tiny /tmp 1234  #use given port, serve given dir\n");
  printf("default port is %d.\n", DEFAULT_PORT);
}

int main(int argc, char **argv) {
  if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
    print_help();
    return 0;
  }

  struct sockaddr_in clientaddr;
  int default_port = DEFAULT_PORT, listenfd, connfd;
  char buf[256];
  char *path = getcwd(buf, 256);
  socklen_t clientlen = sizeof clientaddr;
  if (argc == 2) {
    if (argv[1][0] >= '0' && argv[1][0] <= '9') {
      default_port = atoi(argv[1]);
    } else {
      path = argv[1];
      if (chdir(path) != 0) {
        perror(path);
        exit(1);
      }
    }
  } else if (argc == 3) {
    default_port = atoi(argv[2]);
    path = argv[1];
    if (chdir(path) != 0) {
      perror(path);
      exit(1);
    }
  }
  printf("serve directory '%s'\n", path);

  listenfd = open_listenfd(default_port);
  if (listenfd > 0) {
    printf("listen on port %d, fd is %d\n", default_port, listenfd);
  } else {
    perror("ERROR");
    exit(listenfd);
  }
  // Ignore SIGPIPE signal, so if browser cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);

  int i = 0;
  for (; i < FORK_COUNT; i++) {
    int pid = fork();
    if (pid == 0) {  //  child
      while (1) {
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        process(connfd, &clientaddr);
        close(connfd);
      }
    } else if (pid > 0) {  //  parent
      printf("child pid is %d\n", pid);
    } else {
      perror("fork");
    }
  }

  while (1) {
    connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
    process(connfd, &clientaddr);
    close(connfd);
  }

  return 0;
}
