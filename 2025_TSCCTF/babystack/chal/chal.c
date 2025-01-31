#include <stdio.h>
#include <stdlib.h>

int main() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    char *ptr = 0;
    char ans0[0x10] = {0};
    char ans1[0x10] = {0};
    char ans2[0x10] = {0};

    puts("========= Welcome To Baby Stack =========");
    printf("| Gift : %p\n", puts);

    puts("| Do you know how the stack works ?");
    printf("| > ");
    read(0, ans0, 8);

    puts("| Do you know how the stack works ?");
    printf("| > ");
    read(0, ans1, 8);

    puts("| Do you know how the stack works ?");
    printf("| > ");
    read(0, ans2, 8);

    puts("| Show your skills !");
    printf("| > ");
    scanf("%llx", &ptr);

    printf("| > ");
    read(0, ptr, 0x10);

    puts("========= End Of Baby Stack =========");

    return 0;
}
