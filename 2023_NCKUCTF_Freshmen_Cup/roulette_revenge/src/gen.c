#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void print_flag(){
	char *key = "s3cret1y...N1nLSUnBabY";
	char *enc = "NCKUCTF{Y0U_4R3_R34l1y_1UcKY_gO0d_J0B}\n";
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