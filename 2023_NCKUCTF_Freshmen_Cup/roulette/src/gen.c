#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void print_flag(){
	char *key = "Welcome to NCKU CTF!";
	char *enc = "NCKUCTF{T0O_345Y_yOU_5H0U1d_7RY_r3V3nG3_v3R510n}\n";
    for(int i=0;i<strlen(enc);i++){
        printf("%d, ",enc[i]^key[i%strlen(key)]);
    }
}

char *banner=" _   _  _____ _  ___    _\n" 
 "| \\ | |/ ____| |/ / |  | |\n"
 "|  \\| | |    | ' /| |  | |\n"
 "| . ` | |    |  < | |  | |\n"
 "| |\\  | |____| . \\| |__| |\n"
 "|_| \\_|\\_____|_|\\_\\______/\n";

int main(void){
	print_flag();
	return 0;
}