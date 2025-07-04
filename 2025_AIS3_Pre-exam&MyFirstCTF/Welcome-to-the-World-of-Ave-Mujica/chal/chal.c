#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *banner = "░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░      ░▒▓██████████████▓▒░░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░  \n"
               "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n"
               "░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ \n"
               "░▒▓████████▓▒░░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░        ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓████████▓▒░ \n"
               "░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ \n"
               "░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n"
               "░▒▓█▓▒░░▒▓█▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░      ░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░ \n";

void Welcome_to_the_world_of_Ave_Mujica() {
    puts("🎸 歡迎來到 Ave Mujica 的世界...🎭");
    puts("由 豐川集團 獨家冠名贊助");
    puts("ト（😮T）");
    puts("ガ（😃G）");
    puts("ワ（😉W）");
    puts("グルー（😄👐）プ");
    puts("立希漂亮漂亮漂亮");
    puts("海鈴帥氣帥氣帥氣");
    execve("/bin/sh", 0, 0);
}

int8_t read_int8() {
    char buf[4];
    read(0, buf, 4);
    int x = atoi(buf);
    if (x > 127) {
        puts("我的意思就是你的名字太長了");
        exit(1);
    }
    return x;
}

int main() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    printf("\033[2J\033[1;1H");
    printf("\033[31m");
    printf("%s", banner);
    puts("我是 Ave Mujica 的 Oblivionis (Ob 一串字母女士)");
    puts("你願意把剩餘的人生交給我嗎?");
    char option[8];
    fgets(option, sizeof(option), stdin);
    char *newline = strchr(option, '\n');
    if (newline) {
        *newline = '\0';
    }
    if (strcmp(option, "yes") != 0) {
        puts("🦆打🦆打，賊帶🦆打，開🌂🐸🪑🦆！");
        exit(1);
    }
    printf("告訴我你的名字的長度: ");
    uint8_t x = read_int8();
    char buf[128];
    printf("告訴我你的名字: ");
    read(0, buf, x);
    return 0;
}