#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LONGITUD_COMANDO 100

int main(){

    char *comando[LONGITUD_COMANDO];
    
    for (int i = 0; i < LONGITUD_COMANDO; i++) {
        comando[i] = malloc(100 * sizeof(char));  // Reserva memoria para cada cadena
    }

    printf(">> ");
    scanf("%s", comando[0]);
    while (getchar() != '\n');  // Limpiar el buffer de entrada
    printf("%s\n", comando[0]);

     if(strcmp(comando[0], "hola") == 0){
        printf("Comando: %s", comando[0]);
    }
    else{
        printf("Comando erroneo");
    }

    // Liberar memoria al final del programa
    for (int i = 0; i < LONGITUD_COMANDO; i++) {
        free(comando[i]);
    }

    return 0;

}

