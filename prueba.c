#include <stdio.h>
#include "cabeceras.h"

int main(){

    FILE *fentrada;
    int bytesread;
    typedef struct {
        char nombre[12];
        int nBTC;
    } T_BTC;

    T_BTC regBTC;

    fentrada = fopen("particion.bin", "rb");
    if(fentrada != NULL){
        while((bytesread = fread(&regBTC, sizeof(T_BTC), 1, fentrada)) > 0){
            printf("bitcoins: %d\n",regBTC.nBTC);
        }
    }
    fclose(fentrada);
}