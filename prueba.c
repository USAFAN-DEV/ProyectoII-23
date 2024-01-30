#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    const char* p = "00010000000000000000";
    int* ip = (int*) p;
    int res = 0;

    for(int i = 0; i < 5; i++){
        printf("\n%d", res);
        res += ip[i];
    }

    printf("%d",res);
}