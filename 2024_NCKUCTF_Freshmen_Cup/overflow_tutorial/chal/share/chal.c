#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
struct chal
{
  char buf[0x20];
  char target[8];
} pwnme;

uint8_t read_uint8(){
  // don't use scanf, exit if they try to read negative number
  // only accept from 0 to 255
  char buf[0x8];
  read(0, buf, 0x8);
  int n = atoi(buf);
  if(n < 0){
    puts("Invalid number, only positive number is allowed!");
    exit(0);
  }
  return (uint8_t)n;
}

int main(){
  // Just some IO buffering stuff, don't worry about it in this challenge
  setvbuf(stdin, 0, _IONBF, 0);
  setvbuf(stdout, 0, _IONBF, 0);
  // Challenge starts here
  strncpy(pwnme.target, "pwn2ooo", 8);
  puts("Welcome to introduction to overflow!");
  puts("Now you'll learn both integer and buffer overflow in this challenge");
  uint8_t c = 5;
  puts("First Stage:\nI have a 8-bit unsigned integer (8-bit unsigned integer has range from 0 to 255).\nNow it is 5, can you add some positive number to make it 0?");
  uint8_t n;
  printf("Current number: %hhu\n", c);
  printf("Give me your number: ");
  n = read_uint8();
  c += n;
  printf("Your number: %hhu\n", n);
  printf("c + %hhu is now %hhu\n", n, c);
  if(c != 0){
    puts("Failed, try harder!");
    return 0;
  }
  puts("Congratulations! You've passed the first stage!");
  puts("Explanation: You can overflow the 8-bit unsigned integer by adding a number that makes it greater than 255,\nwhich will make it wrap around to 0");
  puts("In the first stage, if you add 251 to 5, it will be 256, but since it's an 8-bit integer, it will go back to 0.");
  puts("By the way, minus has similar problem. We call it integer overflow.");
  printf("Second Stage:\nNow you have an input buffer at %p\n", pwnme.buf);
  printf("Target is at %p\n", pwnme.target);
  printf("Target now has string: %s\n", pwnme.target);
  puts("Now you can input as long as you like to the input buffer, can you overwrite the target string to NCKUCTF?");
  gets(pwnme.buf);
  printf("Target now has string: %s\n", pwnme.target);
  if(strcmp(pwnme.target, "NCKUCTF") == 0){
    puts("Congratulations! You've passed the second stage!");
    puts("Explanation: You can overflow the buffer by inputting more than the buffer size, which will overwrite the data after the buffer");
    int fd = open("flag", 0);
    if (fd < 0){
      puts("Failed to open flag file, please contact the admin.");
      return 0;
    }
    char flag[100];
    read(fd, flag, 100);
    printf("Now here's your reward: %s\n",flag);
  }
  else{
    puts("Failed, try overflow the buffer harder!\nTips: What's the distance between the buffer and the target?");
  }
}