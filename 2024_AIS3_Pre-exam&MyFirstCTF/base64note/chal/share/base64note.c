#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF 1450
#define MAX_NOTE 8

void menu() {
  puts("1. Add a note in base64.");
  puts("2. Delete a note.");
  puts("3. Print a note.");
  puts("4. Exit");
}

char *bins[MAX_NOTE] = {0};

char tmp_buf[MAX_BUF + 4];

void IsValidbase64Input(char *str, int len) {
  for (int i = 0; i < len; i++) {
    if (!((str[i] >= 'A' && str[i] <= 'Z') ||
          (str[i] >= 'a' && str[i] <= 'z') ||
          (str[i] >= '0' && str[i] <= '9') || str[i] == '+' || str[i] == '/')) {
      puts("Not a valid base64 input format!");
      puts("Tip: Just input a base64 string \"without\" equal sign, thanks!");
      exit(3);
    }
  }
}
char decoding_table[128] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,
    0,  0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51, 0,  0,  0,  0,  0};

// decode base64
char *base64_decode(char *code, int input_len) {
  int output_len = 3 * (input_len / 4) + 1;
  char *res = (char *)malloc(output_len);
  if (res == NULL) {
    puts("Malloc error!");
    exit(4);
  }
  int i = 0, j = 0;
  int tmp = 0;
  int shift = 0;
  // decode base64 for each 4 bytes
  while (i < input_len && j < output_len) {
    int b = decoding_table[(int)code[i++]];
    tmp = (tmp << 6) | b;
    shift++;
    if (shift == 4) {
      res[j++] = (tmp >> 16) & 0xFF;
      res[j++] = (tmp >> 8) & 0xFF;
      res[j++] = tmp & 0xFF;
      shift = 0;
      tmp = 0;
      continue;
    }
  }
  // handle the last 1-3 bytes if any
  switch (shift) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      res[j++] = (tmp >> 4) & 0xFF;
      break;
    case 3:
      res[j++] = (tmp >> 10) & 0xFF;
      res[j++] = (tmp >> 2) & 0xFF;
      break;
    default:
      break;
  }
  return res;
}

void add_note() {
  int idx = 0;
  puts("Which idx would you like to add a note?");
  printf("> ");
  scanf("%d", &idx);
  getchar();
  if (idx >= MAX_NOTE || idx < 0) {
    puts("Invalid idx!");
    exit(-1);
  }
  puts("Describe your new note in base64!");
  puts("What is the length of your description? (After base64 encoding)");
  printf("> ");
  int size = 0;
  scanf("%d", &size);
  getchar();
  if (size > MAX_BUF || size < 0) {
    puts("Note size error!");
    exit(-1);
  }
  memset(tmp_buf, 0, sizeof(tmp_buf));
  puts("Enter your description in base64: ");
  printf("> ");
  read(0, tmp_buf, size);
  // check vaild base64 or not
  IsValidbase64Input(tmp_buf, size);
  // copy to bins
  bins[idx] = base64_decode(tmp_buf, size);
  puts("Done!");
}

// remove a note
void remove_note() {
  int idx = 0;
  puts("Which note would you like to remove?");
  printf("> ");
  scanf("%d", &idx);
  getchar();
  if (idx >= MAX_NOTE || idx < 0) {
    puts("Invalid idx!");
    exit(-1);
  }
  free(bins[idx]);
  puts("Done!");
}

// print a note
void print_note() {
  int idx = 0;
  puts("Which note would you like to print?");
  printf("> ");
  scanf("%d", &idx);
  getchar();
  if (idx >= 8 || idx < 0) {
    puts("Invalid idx!");
    exit(-1);
  }
  if (bins[idx]) puts(bins[idx]);
  puts("Done!");
}

int main(int argc, char **argv) {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  puts("Base64 Nooote (under development)");
  puts("If you find any bug, just exploooit it!");
  while (1) {
    menu();
    printf("> ");
    int choice;
    scanf("%d", &choice);
    getchar();
    switch (choice) {
      case 1:
        add_note();
        break;
      case 2:
        remove_note();
        break;
      case 3:
        print_note();
        break;
      case 4:
        puts("Giving up?");
        exit(0);
      default:
        puts("Invalid choice. Please try again!");
        break;
    }
  }
  exit(0);
}