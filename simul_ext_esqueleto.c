#include<stdio.h>
#include<string.h>
#include<ctype.h>
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
     
     // Buce de tratamiento de comandos
   for (;;){
		do {
		   printf (">> ");
		   fflush(stdin);
		   fgets(comando, LONGITUD_COMANDO, stdin);
		}
      while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
	   if (strcmp(orden,"dir")==0) {
         Directorio(&directorio,&ext_blq_inodos);
         continue;
      }
      //...
      // Escritura de metadatos en comandos rename, remove, copy     
      Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
      GrabarByteMaps(&ext_bytemaps,fent);
      GrabarSuperBloque(&ext_superblock,fent);
      if (grabardatos)
         GrabarDatos(&memdatos,fent);
      grabardatos = 0;
      //Si el comando es salir se habrán escrito todos los metadatos
      //faltan los datos y cerrar
      if (strcmp(orden,"salir")==0){
         GrabarDatos(&memdatos,fent);
         fclose(fent);
         return 0;
      }
   }
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
   

}
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
//Aqui lo que tiene que hacer es recibir el comando la orden registra si es dir, imprimir o lo que sea y despues argumento 1 y 2 es por si 
//tiene que hacer cp A.txt D.txt
   int valor=1;
   char *token;
   orden=strtok(strcomando," ");
   if(strcmp(orden,"copy")==0 || strcmp(orden,"rename"==0)){
      argumento1=strtok(NULL," ");
      argumento2=strtok(NULL," ");
      valor=0;
   }
   else if(strcmp(orden,"imprimir")==0 || strcmp(orden,"remove")==0){
      argumento1=strtok(NULL," ");
      valor=0;
   }
   else if(strcmp(orden,"info")==0 || strcmp(orden,"bytemaps")==0){
      valor=0;
   }
   else{
      printf("ERROR: comando ilegal [bytempas,copy,dir,info,imprimir,rename,remove,salir]\n");
   }
   return valor;
   
}
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
   printf("%d\n%d\n",psup->s_block_size,psup->s_blocks_count);

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
