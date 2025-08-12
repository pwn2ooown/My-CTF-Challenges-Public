#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define MAX_BIN 8

void menu() {
  puts("1. Get a superpower");
  puts("2. Remove a superpower");
  puts("3. Print a superpower");
  puts("4. Update a superpower");
  puts("5. Exit");
}

char *bins[MAX_BIN] = {0};
uint32_t len[MAX_BIN] = {0};

int get_total_len() {
  for (int i = 0; i < MAX_BIN; ++i) {
    if (!bins[i]) return i;
  }
  return -1;
}

void add_power() {
  int index = get_total_len();
  if (index < 0) {
    puts("You have too many powers!");
    exit(-1);
  }
  puts("Describe your new power.");
  puts("What is the length of your description?");
  printf("> ");
  unsigned int size = 0;
  scanf("%u", &size);
  getchar();
  bins[index] = (char *)malloc(size);
  if (bins[index] == NULL) {
    perror("malloc");
    exit(-1);
  }
  len[index] = size;
  puts("Enter your description: ");
  printf("> ");
  fgets(bins[index], size, stdin);
  puts("Done!");
}

void remove_power() {
  int index = 0;
  puts("Which power would you like to remove?");
  printf("> ");
  scanf("%d", &index);
  getchar();
  if (index >= MAX_BIN || index < 0) {
    puts("Invalid index!");
    exit(-1);
  }
  if (bins[index] == NULL) {
    puts("You don't have that power!");
    exit(-1);
  }
  free(bins[index]);
  // bins[index] = NULL; // vulnerability here
  // len[index] = 0;
  puts("Done!");
}

void print_power() {
  int index = -1;
  puts("Which power would you like to see?");
  printf("> ");
  scanf("%d", &index);
  getchar();
  if (index >= MAX_BIN || index < 0) {
    puts("Invalid index!");
    exit(-1);
  }
  if (bins[index] == NULL) {
    puts("You don't have that power!");
    exit(-1);
  }
  printf("Your superpower is: %s\n", bins[index]);
}

void update_power() {
  int index = -1;
  puts("Which power would you like to update?");
  printf("> ");
  scanf("%d", &index);
  getchar();
  if (index >= MAX_BIN || index < 0) {
    puts("Invalid index!");
    exit(-1);
  }
  if (bins[index] == NULL) {
    puts("You don't have that power!");
    exit(-1);
  }
  puts("Enter your new description: ");
  printf("> ");
  fgets(bins[index], len[index], stdin);
  puts("Updated!");
}

int main(int argc, char **argv) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  puts("From Zero to Hero - Nerf Version 2");
  puts("So, you wanna be a hero?");
  char opt[2];
  read(0, opt, 2);
  if (opt[0] == 'y') {
    puts("Really? Being a hero is hard.");
    puts("Fine. I see I can't convince you otherwise.");
  } else {
    puts("No? Then why are you even here?");
    exit(0);
  }

  while (1) {
    menu();
    printf("> ");
    int choice;
    scanf("%d", &choice);
    getchar();
    switch (choice) {
      case 1:
        add_power();
        break;
      case 2:
        remove_power();
        break;
      case 3:
        print_power();
        break;
      case 4:
        update_power();
        break;
      case 5:
        puts("Giving up?");
        exit(0);
      default:
        puts("Invalid choice. You are not a hero.");
        break;
    }
  }
  return 0;
}
