#include <stdio.h>
#include <unistd.h>
void call_me(){
  execve("/bin/sh",NULL,NULL);
}

int main(){
  setvbuf(stdin, 0, _IONBF, 0);
  setvbuf(stdout, 0, _IONBF, 0);
  char buf[0x20];
  puts("Welcome to NCKUCTF Freshmen cup!");
  gets(buf);
}