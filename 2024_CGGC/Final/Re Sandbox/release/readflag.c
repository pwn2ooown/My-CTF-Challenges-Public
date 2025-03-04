#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    if(argc < 5) {
        printf("Usage: %s give me the flag\n", argv[0]);
        return 1;
    }

    if ((strcmp(argv[1], "give") | strcmp(argv[2], "me") | strcmp(argv[3], "the") | strcmp(argv[4], "flag")) != 0) {
        puts("Nope!");
        return 1;
    }

    char flag[256] = { 0 };
    FILE* fp = fopen("/flag", "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }
    if (fread(flag, 1, 256, fp) < 0) {
        perror("fread");
        return 1;
    }
    puts(flag);
    fclose(fp);
    return 0;
}
