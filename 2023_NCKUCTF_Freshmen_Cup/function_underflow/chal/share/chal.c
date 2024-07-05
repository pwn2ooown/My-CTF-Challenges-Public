// gcc chal.c -o chal

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef void (*func_ptr)(void);

#define FUNC_NUM 5
#define FLAGSIZE 64
void print() { puts("Welcome to NCKU CTF!"); }

void inp() {
  char buf[10] = {};
  printf("Name: ");
  read(0, buf, 10);
  printf("Welcome to NCKU CTF!, %s\n", buf);
}

void rickroll() {
  puts("Never gonna give you up");
  puts("Never gonna let you down");
  puts("Never gonna run around and desert you");
  puts("Never gonna make you cry");
  puts("Never gonna say goodbye");
  puts("Never gonna tell a lie and hurt you");
}

void no_flag() {
  puts("No flag for you!");
}
void admin() {
  system("cat /home/chal/flag");
}
int read_int_lower_than(int bound) {
  int x;
  scanf("%d", &x);
  if (x >= bound) {
    puts("Invalid input!");
    exit(1);
  }
  return x;
}
int main(void) {
  func_ptr funs[FUNC_NUM] = {print, inp, rickroll, no_flag, admin};
  puts("Which function do you want to call?");
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  unsigned char idx = read_int_lower_than(FUNC_NUM - 1);
  funs[idx]();
  return 0;
}