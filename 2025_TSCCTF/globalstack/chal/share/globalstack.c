#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define MAX_STACK_SIZE 20
#define MAX_INPUT_SIZE 25
int64_t stack[MAX_STACK_SIZE];
int64_t* top = stack - 1;

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    char *input = malloc(MAX_INPUT_SIZE);
    char *command = malloc(MAX_INPUT_SIZE);
    int64_t value;
    puts("Commands: 'push <value>', 'pop', 'show', 'help', or 'exit'");

    while (1) {
        printf(">> ");
        fgets(input, MAX_INPUT_SIZE, stdin);
        sscanf(input, "%s", command);

        if (strcmp(command, "push") == 0) {
            if (sscanf(input, "%*s %ld", &value) == 1) {
                top += 1;
                *top = (int64_t)value;
                printf("Pushed %ld to stack\n", value);
            } else {
                printf("Invalid push.\n");
            }
        } else if (strcmp(command, "pop") == 0) {
            printf("Popped %ld from stack\n", *top);
            top -= 1;
        } else if (strcmp(command, "show") == 0) {
            printf("Stack top: %ld\n", *top);
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
        else if (strcmp(command, "help") == 0) {
            puts("Commands: 'push <value>', 'pop', 'show', 'help', or 'exit'");
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
    free(input);
    free(command);
    return 0;
}