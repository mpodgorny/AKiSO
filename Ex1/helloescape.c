#include <stdio.h>



int main()
{
    for(int i=0;i<256;i++){
        printf("\033[38;5;%dmHello, world!",i);
    }

    return 0;
}
