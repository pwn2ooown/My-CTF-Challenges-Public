#include <stdio.h>

int main(int argc, char **argv) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  char buf[32];
  read(0, buf, 0x30);
  puts("Mission failed?");
  return 0;
}