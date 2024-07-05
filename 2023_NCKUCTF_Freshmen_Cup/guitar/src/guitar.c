#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_flag() {
  char *flag = "NCKUCTF{do_you_know_what_string_has_md5_4c39e90d6a5c38a3f8a9b1f05840f240}";
  puts(flag);
}
#include <time.h>
char *banner =
    " _   _  _____ _  ___    _\n"
    "| \\ | |/ ____| |/ / |  | |\n"
    "|  \\| | |    | ' /| |  | |\n"
    "| . ` | |    |  < | |  | |\n"
    "| |\\  | |____| . \\| |__| |\n"
    "|_| \\_|\\_____|_|\\_\\______/\n";
#include <string.h>
char *fake_flag = "NOTFLAG{do_you_know_what_is_grep}";
int main(void) {
  puts(banner);
  puts("Is bass a four string guitar?");
  char opt[2];
  read(0, opt, 2);
  if (opt[0] == 'y' || opt[0] == 'Y') {
    puts("Checkmate!");
    sleep(0xc8763);
    print_flag();
  }
  else{
    puts("You are wrong! Slap.");
  }
  return 0;
}