#include <stdio.h>

int main()
{
    for(int i=0;i>=-65536;i--){
        if((unsigned char)i == 4){
            printf("%d",i); // -252
            break;
        }
    }

    return 0;
}