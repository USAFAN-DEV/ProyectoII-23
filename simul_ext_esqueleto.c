#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo, EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
char* LeelineaDinamica();

int main(){

	char *comando[LONGITUD_COMANDO];
	char *orden[LONGITUD_COMANDO];
	char *argumento1[LONGITUD_COMANDO];
	char *argumento2[LONGITUD_COMANDO];
	 
   int i,j;
   unsigned long int m;
   EXT_SIMPLE_SUPERBLOCK ext_superblock; //SUPERBLOQUE
   EXT_BYTE_MAPS ext_bytemaps; //BYTEMAPS
   EXT_BLQ_INODOS ext_blq_inodos;  //INODOS
   EXT_ENTRADA_DIR directorio[MAX_FICHEROS]; //FICHEROS
   EXT_DATOS memdatos[MAX_BLOQUES_DATOS]; 
   EXT_DATOS datosfich[MAX_BLOQUES_PARTICION]; 
   int entradadir;
   int grabardatos;
   int contadorComandos = -1; //Inicializado a -1 en vez de a 0 por donde empezamos a aumentar el contador
   FILE *fent;
   
   // Lectura del fichero completo de una sola vez
   //...
   
   fent = fopen("particion.bin","r+b");
   fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent); //devuelve el número de elementos completos que lee la función
   
   memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
   memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
   memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
   memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
   memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

   //Reserva de memoria dinamica
   for (int i = 0; i < LONGITUD_COMANDO; i++) {
      comando[i] = malloc(100 * sizeof(char)); 
      orden[i] = malloc(100 * sizeof(char)); 
      argumento1[i] = malloc(100 * sizeof(char)); 
      argumento2[i] = malloc(100 * sizeof(char)); 
   }

   // Bucle de tratamiento de comandos
   for (;;){
      do {
         int cont = 0;
         char c;
         contadorComandos++;
         printf ("\n>> ");
         while((c = getchar()) != '\n'){
            comando[contadorComandos][cont] = c;
            cont++;
         }
         printf("El comando leido es: %s", comando[contadorComandos]);
         
      } while(ComprobarComando(comando[contadorComandos],*orden,*argumento1,*argumento2) !=0);


      /*
	   if (strcmp(*orden,"dir")==0) {
         Directorio(directorio,&ext_blq_inodos);
         continue;
      }
      else if(strcmp(*orden,"copy")==0){
         Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
            GrabarDatos(memdatos,fent);
         grabardatos = 0;
      }
      else if(strcmp(*orden,"rename")==0){
         Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
            GrabarDatos(memdatos,fent);
         grabardatos = 0;
      }
      else if(strcmp(*orden,"imprimir")==0){

      }
      else if(strcmp(*orden,"remove")==0){
         Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
            GrabarDatos(memdatos,fent);
         grabardatos = 0;
      }
      else if(strcmp(*orden,"imprimir")==0){

      }
      else if(strcmp(*orden,"info")==0){
         LeeSuperBloque(&ext_superblock);
      }
      else if(strcmp(*orden,"bytemaps")==0){
         Printbytemaps(&ext_bytemaps);
      }
      else{
         printf("ERROR: comando ilegal [bytempas,copy,dir,info,imprimir,rename,remove,salir]\n");
      }
      */
      //...
      // Escritura de metadatos en comandos rename, remove, copy     
      
      //Si el comando es salir se habrán escrito todos los metadatos
      //faltan los datos y cerrar
      if (strcmp(*orden,"salir")==0){
         GrabarDatos(memdatos,fent);
         fclose(fent);
         // Liberar memoria al final del programa
         for (int i = 0; i < LONGITUD_COMANDO; i++) {
            free(comando[i]);
            free(orden[i]);
            free(argumento1[i]);
            free(argumento2[i]);
         }
         return 0;
      }
   }
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
   printf("\nInodos: ");
   for(int i = 0; i < MAX_INODOS; i++){
      printf("%d ", ext_bytemaps->bmap_inodos[i]);
   }
   printf("\nBloques[0-25]: ");
   for(int i = 1; i < 26; i++){
      printf("%d ", ext_bytemaps->bmap_bloques[i]);
   }
}
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
//Aqui lo que tiene que hacer es recibir el comando la orden registra si es dir, imprimir o lo que sea y despues argumento 1 y 2 es por si 
//tiene que hacer cp A.txt D.txt
   int valor=1;

   //Parseamos el comando
   if((orden = strtok(strcomando, " ")) == NULL){
      orden = strcomando;
   }
   else{
      if((argumento1 = strtok(NULL, " ")) != NULL){
         if((argumento2 = strtok(NULL, " ")) != NULL);
         else{
            argumento2 = NULL;
         }
      }
      else{
         argumento2 = NULL;
      }
   }

   printf("\nLa orden recibida es: %s", orden);
   printf("\nEl argumento1 es: %s", argumento1);
   printf("\nEl argumento2 es: %s\n", argumento2);


  /*
   if(strcmp(orden,"copy")==0 || strcmp(orden,"rename")==0){
      printf("\nEl comando es copy o rename");
      argumento1=strtok(NULL," ");
      argumento2=strtok(NULL," ");
      valor=0;
   }
   else if(strcmp(orden,"imprimir")==0 || strcmp(orden,"remove")==0){
      printf("\nEl comando es imprimir o remove");
      argumento1=strtok(NULL," ");
      valor=0;
   }
   else if(strcmp(orden,"info")==0 || strcmp(orden,"bytemaps")==0){
      printf("\nEl comando es info o bytemaps");
      valor=0;
   }
   */

   return valor;
}


void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
   printf("Bloque %d Bytes\ninodos particion = %d\ninodos libres = %d\nBloques particion = %d\nBloques libres = %d\nPrimer bloque de datos = %d",psup->s_block_size, psup->s_inodes_count, psup->s_free_inodes_count, psup->s_blocks_count, psup->s_free_blocks_count, psup->s_first_data_block);

}
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){



}
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){


}
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo, EXT_DATOS *memdatos, char *nombre){


}
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich){


}
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich){


}
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich){


}
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich){


}
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich){


}
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich){


}

char* LeelineaDinamica(){
    char caracter;
    int contador=0,numbloque=1;
    char *linea;
    linea=(char*)malloc(10);
    while((caracter=getchar())!='\n'){
        if(contador>=numbloque * 10){
            numbloque++;
            linea=(char*)realloc(linea,numbloque * 10);
        }
        linea[contador]=caracter;
        contador++;
    }
    linea[contador]='\0';
    return linea;
}
