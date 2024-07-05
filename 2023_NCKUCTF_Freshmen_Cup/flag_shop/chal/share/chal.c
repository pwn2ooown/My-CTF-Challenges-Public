#include <stdio.h>

void menu() {
  puts("1. Buy a fake flag for 69");
  puts("2. Buy flag for 0x13371337");
}
char *banner =
    " _   _  _____ _  ___    _\n"
    "| \\ | |/ ____| |/ / |  | |\n"
    "|  \\| | |    | ' /| |  | |\n"
    "| . ` | |    |  < | |  | |\n"
    "| |\\  | |____| . \\| |__| |\n"
    "|_| \\_|\\_____|_|\\_\\______/\n";
int main() {
  setvbuf(stdin, 0, _IONBF, 0);
  setvbuf(stdout, 0, _IONBF, 0);
  puts(banner);
  puts("Welcome to NCKU flag shop.");
  int money = 1337;
  while (1) {
    int choice;
    menu();
    printf("You have %d dollars\n", money);
    printf(">> ");
    scanf("%d", &choice);
    if (choice == 1) {
      int num;
      puts("How many?");
      printf(">> ");
      scanf("%d", &num);
      money -= 69 * num;
      puts("Here is your fake flag");
      puts("NOTFLAG{you_need_to_buy_a_real_flag}");
    } else if (choice == 2) {
      if (money >= 0x13371337) {
        system("cat /home/chal/flag");
        return 0;
      } else {
        puts("You don't have enough money");
      }
    } else {
      puts("Invalid choice");
      break;
    }
  }

  return 0;
}