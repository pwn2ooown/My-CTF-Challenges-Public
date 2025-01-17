#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define MAX_STACK_SIZE 20
#define MAX_INPUT_SIZE 25

void print_flag() {
    char flag[64];
    FILE *f = fopen("flag", "r");
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }
    fgets(flag, sizeof(flag), f);
    printf("%s",flag);
    fclose(f);
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    char input[25];
    char command[25];
    int64_t stack[MAX_STACK_SIZE];
    int64_t top = -1;
    int64_t value;
    puts("Commands: 'push <value>', 'pop', 'show', 'help', or 'exit'");

    while (1) {
        printf(">> ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", command);

        if (strcmp(command, "push") == 0) {
            if (sscanf(input, "%*s %ld", &value) == 1) {
                stack[++top] = value;
                printf("Pushed %ld to stack\n", value);
            } else {
                printf("Invalid push.\n");
            }
        } else if (strcmp(command, "pop") == 0) {
            printf("Popped %ld from stack\n", stack[top--]);
        } else if (strcmp(command, "show") == 0) {
            printf("Stack top: %ld\n", stack[top]);
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
        else if (strcmp(command, "help") == 0) {
            puts("Commands: 'push <value>', 'pop', 'show', 'help', or 'exit'");
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
    return 0;
}