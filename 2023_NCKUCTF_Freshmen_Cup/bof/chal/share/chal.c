#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 32
#define FLAGSIZE 64

void win() {
  char *argv[20] = {"/bin/cat", "/home/chal/flag"};
  execve("/bin/cat", argv , NULL);
}

void vuln() {
  char buf[BUFSIZE];
  gets(buf);
}

int main(int argc, char **argv) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  puts("Do you know how to ROP?");
  puts("Here's your gift:");
  char* cmd = "/bin/date";
  system(cmd);
  puts("Now, show me your skills.");
  vuln();
  return 0;
}