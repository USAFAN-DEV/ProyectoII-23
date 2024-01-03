#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char **orden, char **argumento1, char **argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo, EXT_DATOS *memdatos);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);
char* LeelineaDinamica();

int main(){

	char *comando;
	char *orden;
	char *argumento1;
	char *argumento2;
	 
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
   int contadorComandos = 0;
   FILE *fent;
   
   //Lectura del fichero completo de una sola vez
   fent = fopen("particion.bin","r+b"); //abrir archivo binario para lectura y grabacion
   if(fent == NULL){
      printf("\nEl archivo binario no existe. Revise que el nombre este bien escrito");
      exit(0);
   }

   fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent); //devuelve el número de elementos completos que lee la función
   memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
   memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
   memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
   memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
   memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

   //Reserva de memoria dinamica
   comando = (char*)malloc(100*sizeof(char));
   orden = (char*)malloc(100*sizeof(char));
   argumento1 = (char*)malloc(100*sizeof(char));
   argumento2 = (char*)malloc(100*sizeof(char));

   //Bucle de tratamiento de comandos
   for (;;){

      do {
         contadorComandos = 0;

         printf ("\n>> ");
         comando = LeelineaDinamica();

         if(contadorComandos == 100){
            printf("\nLa longitud maxima de un comando es %d", LONGITUD_COMANDO);
         }

      } while(ComprobarComando(comando, &orden, &argumento1, &argumento2) !=1);

      if(strcmp(orden, "info") == 0){
         LeeSuperBloque(&ext_superblock);
      }
      else if(strcmp(orden, "bytemaps") == 0){
         Printbytemaps(&ext_bytemaps);
      }
      else if(strcmp(orden, "dir") == 0){
         Directorio(directorio, &ext_blq_inodos);
      }
      else if(strcmp(orden, "rename") == 0){
         Renombrar(directorio, &ext_blq_inodos, argumento1, argumento2, memdatos);
      }
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
      if (strcmp(orden,"salir")==0){
         GrabarDatos(memdatos,fent);
         fclose(fent);
         
         //Liberamos la memoria dinamica
         free(comando);
         free(orden);
         free(argumento1);
         free(argumento2);

         return 0;
      }
   }
}


int ComprobarComando(char *strcomando, char **orden, char **argumento1, char **argumento2){
//Aqui lo que tiene que hacer es recibir el comando la orden registra si es dir, imprimir o lo que sea y despues argumento 1 y 2 es por si 
//tiene que hacer cp A.txt D.txt
   int comandoCorrecto = 0;

   if((*orden = strtok(strcomando, " ")) == NULL){
      //Si el usuario no escribe nada
      printf("\nERROR. Escribe un comando\n");
   }
   else{
      //Si el usuario escribe algo, parseamos el comando
      if((*argumento1 = strtok(NULL, " ")) != NULL){
         if((*argumento2 = strtok(NULL, " ")) != NULL);
         else{
            *argumento2 = NULL;
         }
      }
      else{
         *argumento2 = NULL;
      }

      //Comprobamos el comando
      if(strcmp(*orden, "copy") == 0 || strcmp(*orden, "rename") == 0){
         if(*argumento1 == NULL || *argumento2 == NULL){
            printf("\nERROR. El comando escrito necesita dos parámetros\n");
         }
         else{
            comandoCorrecto = 1;
         }
      }
      else if(strcmp(*orden,"imprimir")==0 || strcmp(*orden,"remove")==0){
         if(*argumento1 == NULL || *argumento2 != NULL){
            printf("\nERROR. El comando escrito solo puede tener un parámetro\n");
         }
         else{
            comandoCorrecto = 1;
         }
      }
      else if(strcmp(*orden,"info") == 0 || strcmp(*orden,"bytemaps") == 0 || strcmp(*orden, "dir") == 0 || strcmp(*orden, "salir") == 0){
         if(*argumento1 == NULL && *argumento2 == NULL){
            comandoCorrecto = 1;
         }
         else{
            printf("\nERROR. El comando escrito no puede tener parámetros\n");
         }
      }
      else{
         printf("\nERROR. El comando escrito no existe. Vuelva a intentarlo por favor. Comandos disponibles: ");
         printf("\n\ninfo: Este comando muestra la información del superbloque por pantalla");
         printf("\nbytemaps: Este comando muestra el contenido del bytemap de inodos y los 25 primeros elementos del bytemap de bloques.");
         printf("\ndir: Lista todos los ficheros (excepto la entrada especial del directorio raíz)");
         printf("\nrename <nombre antiguo del fichero> <nuevo nombre del fichero: Cambia el nombre de un fichero");
         printf("\nimprimir <nombre del fichero que quieres imprimir>: imprime el contenido de un fichero");
         printf("\nremove <fichero que quieres borrar>: Elimina el fichero");
         printf("\ncopy <nombre del fichero nuevo> <nombre del fichero del que quieres copiar el contenido>: Crea un nuevo fichero y copia en el el contenido de otro fichero");
         printf("\nsalir: cierra la sesion\n");
      }
   }


   return comandoCorrecto;
}


void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
   //Funcion que se ejecuta con el comando info

   printf("Bloque %d Bytes\ninodos particion = %d\ninodos libres = %d\nBloques particion = %d\nBloques libres = %d\nPrimer bloque de datos = %d\n",psup->s_block_size, psup->s_inodes_count, psup->s_free_inodes_count, psup->s_blocks_count, psup->s_free_blocks_count, psup->s_first_data_block);

}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
   //Funcion que se ejecuta con el comando bytemaps

   printf("\nInodos: ");
   for(int i = 0; i < MAX_INODOS; i++){
      printf("%d ", ext_bytemaps->bmap_inodos[i]);
   }
   printf("\nBloques[0-25]: ");
   for(int i = 0; i < 26; i++){
      printf("%d ", ext_bytemaps->bmap_bloques[i]);
   }

   printf("\n");
}


void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos) {
   // Funcion que se ejecuta con el comando dir

   for (int i = 1; i < MAX_FICHEROS; i++) {
      if (directorio[i].dir_inodo == 0xFFFF) {
         continue;
      } else {
         printf("\n%s\ttamaño: %u\tinodo:%u\tbloques: ", directorio[i].dir_nfich, inodos->blq_inodos[directorio[i].dir_inodo].size_fichero, directorio[i].dir_inodo);
         for (int j = 0; j < MAX_NUMS_BLOQUE_INODO; j++) {
            if (inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j] == 0xFFFF) {
               continue;
            } else {
               printf("%u ", inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]);
            }
         }
         printf("\n");
      }
   }
}

int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo, EXT_DATOS *memdatos){

   //Por default, el fichero al que le queremos cambiar el nombre no existe y el nuevo nombre esta disponible
   int existeFichero = 0;
   int nombreDisponible = 1;
   int renombrarPosible = 0;

   int indexFichero = -1;

   for(int i = 0; i < MAX_FICHEROS; i++){ //Comprobar que el nombre antiguo es el nombre de un fichero de nuestra particion
      if(strcmp(nombreantiguo, directorio[i].dir_nfich) == 0){
         existeFichero = 1; 
         indexFichero = i;
         i = MAX_FICHEROS; //para salir del bucle
      }
   }
   if(existeFichero == 1){ //Si el fichero con el nombre antiguo existe, comprobar que el nombre nuevo no es el nombre de ningun fichero de nuestra particion
      for(int i = 0; i < MAX_FICHEROS; i++){
         if(strcmp(nombrenuevo, directorio[i].dir_nfich) == 0){
            nombreDisponible = 0;
            i = MAX_FICHEROS; //para salir del bucle
         }
      }
   }
   else;

   if(existeFichero == 0){
      printf("\nERROR. Fichero %s no encontrado\n", nombreantiguo);
   }
   else if(nombreDisponible == 0){
      printf("\nERROR. El fichero %s ya existe\n", nombrenuevo);
   }
   else if(existeFichero == 1 && nombreDisponible == 1){
      for(int i = 0; i < LEN_NFICH; i++){
         directorio[indexFichero].dir_nfich[i] = nombrenuevo[i];
      }
   }

   return indexFichero;

}


int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){


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
