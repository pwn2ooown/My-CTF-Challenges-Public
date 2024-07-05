#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void print_flag() {
  char *key = "Welcome to NCKU CTF!";
  char enc[] = {25,  38,  39,  54, 44,  57, 35,  91,  32, 95, 111, 17, 112,
                127, 96,  121, 28, 45,  9,  116, 8,   80, 36, 83,  58, 92,
                1,   127, 67,  61, 121, 17, 49,  120, 3,  19, 45,  19, 117,
                126, 33,  86,  62, 86,  94, 93,  11,  93, 126};
  for (int i = 0; i < strlen(enc); i++) {
    printf("%c", enc[i] ^ key[i % strlen(key)]);
  }
}

char *banner =
    " _   _  _____ _  ___    _\n"
    "| \\ | |/ ____| |/ / |  | |\n"
    "|  \\| | |    | ' /| |  | |\n"
    "| . ` | |    |  < | |  | |\n"
    "| |\\  | |____| . \\| |__| |\n"
    "|_| \\_|\\_____|_|\\_\\______/\n";

int main(void) {
  printf("%s", banner);
  puts("Welcome to NCKU roulette.");
  puts("Are you lucky enough to guess our number on the roulette?");
  int n;
  srand(time(NULL));
  scanf("%d", &n);
  int target = rand() % 37;
  if (n == target) {
    print_flag();
  } else {
    puts("Better luck next time!");
  }
  return 0;
}