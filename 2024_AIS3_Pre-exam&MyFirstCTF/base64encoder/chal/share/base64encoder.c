#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Vincent55Orz(){
  puts("Here's a shell from NASA hacked Vincent55.🛐🛐🛐");
  system("sh");
}

void base64_encoding(const char *table, char *buf, size_t len) {
  printf("Result: ");
  const int append_mod[3] = {0, 2, 3};
  const int mod[3] = {0, 2, 1};
  int out = 4 * (len / 3) + append_mod[len % 3];
  for (size_t i = 0; i < len;) {
    int32_t tmp_a = i < len ? buf[i++] : 0;
    int32_t tmp_b = i < len ? buf[i++] : 0;
    int32_t tmp_c = i < len ? buf[i++] : 0;
    int32_t combined = (tmp_a << 0x10) + (tmp_b << 0x08) + tmp_c;
    // printf("%d %d %d %d\n",(combined >> 18) % 64,(combined >> 12) % 64,(combined >> 6) % 64,(combined >> 18) % 64);
    if (out) {
      putchar(table[(combined >> 18) % 64]);
      out--;
    }
    if (out) {
      putchar(table[(combined >> 12) % 64]);
      out--;
    }
    if (out) {
      putchar(table[(combined >> 6) % 64]);
      out--;
    }
    if (out) {
      putchar(table[(combined >> 0) % 64]);
      out--;
    }
  }
  for (int i = 0; i < mod[len % 3]; i++) putchar('=');
  putchar('\n');
}

int main() {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  char buf[0x40];
  const char table[0x40] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
  while (1) {
    printf("Text: ");
    memset(buf, 0, sizeof(buf));
    if (scanf("%[^\n]%*c", buf) != 1) return 0;
    base64_encoding(table, buf, strlen(buf));
  }
  return 0;
}
