#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void print_flag(){
	char *hint = "Hint: Did you try some decompiler?";
	char *key = "s3cret1y...N1nLSUnBabY";
	char enc[] = {61, 112, 40, 39, 38, 32, 119, 2, 119, 30, 123, 17, 5, 60, 127, 12, 7, 93, 118, 13, 83, 32, 44, 2, 54, 17, 46, 45, 110, 30, 97, 30, 74, 17, 123, 94, 14, 46, 95};
	for(int i=0;i<strlen(enc);i++){
        printf("%c",enc[i]^key[i%strlen(key)]);
    }
}

char *banner=" _   _  _____ _  ___    _\n" 
 "| \\ | |/ ____| |/ / |  | |\n"
 "|  \\| | |    | ' /| |  | |\n"
 "| . ` | |    |  < | |  | |\n"
 "| |\\  | |____| . \\| |__| |\n"
 "|_| \\_|\\_____|_|\\_\\______/\n";

int main(void){
	printf("%s", banner);
	puts("Welcome to NCKU roulette.");
	puts("Are you lucky enough to guess our number on the roulette?");
	int n;
	srand(time(NULL));
	scanf("%d",&n);
	int target = rand() % 37;
	if (n == target){
		print_flag();
	}
	else{
		puts("Better luck next time!");
	}
	return 0;
}