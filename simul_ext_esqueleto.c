#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps); //Imprimir los bytemaps
int ComprobarComando(char *strcomando, char **orden, char **argumento1, char **argumento2); //Comprobar la sintaxis de los comandos
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup); //Imprimir la informacion del superbloque
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre); //Buscar un fichero en el directorio
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos); //Imprimir los ficheros del directorio
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo, EXT_DATOS *memdatos); //Cambiar el nombre de un fichero
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char* nombre); //Imprimir los datos de un fichero
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich); //Eliminar un fichero
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich); //Copiar los datos de un fichero en otro

//Funciones para guardar el estado de la particion
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

char* LeelineaDinamica();//Funcion para leer una linea

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
   int contadorCaracteresComando = 0;
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
         contadorCaracteresComando = 0;

         printf ("\n>> ");
         comando = LeelineaDinamica();

         while(comando[contadorCaracteresComando] != '\0'){
            contadorCaracteresComando++;

            if(contadorCaracteresComando == 100){
               printf("\nERROR. Los comandos no pueden tener mas de %d caracteres", LONGITUD_COMANDO);
               exit(0);
            }
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
         Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
            GrabarDatos(memdatos,fent);
         grabardatos = 0;
      }
      else if(strcmp(orden, "copy") == 0){
         Copiar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,memdatos,argumento1,argumento2,fent);
         Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
            GrabarDatos(memdatos,fent);
         grabardatos = 0;
      }
      else if(strcmp(orden, "imprimir") == 0){
         Imprimir(directorio, &ext_blq_inodos, memdatos, argumento1);
      }
      else if(strcmp(orden, "remove") == 0){
         Borrar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,argumento1,fent);
         Grabarinodosydirectorio(directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
            GrabarDatos(memdatos,fent);
         grabardatos = 0;
      }
      
      //Si el comando es salir se habrán escrito todos los metadatos
      //faltan los datos y cerrar
      if (strcmp(orden,"salir")==0){
         printf("Antes de grabar datos");
         GrabarDatos(memdatos,fent);
         printf("Despues de grabar datos");     

         //Liberamos la memoria dinamica
         free(comando);
         free(orden);
         free(argumento1);
         free(argumento2);
         fclose(fent);

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

   int existeFichero, nombreDisponible, renombrarPosible = 0;

   existeFichero = BuscaFich(directorio, inodos, nombreantiguo); // -1 si no lo encuentra, su indice si lo encuentra
   nombreDisponible = BuscaFich(directorio, inodos, nombrenuevo); 

   //Lo que nos interesa es cuando el nombre antiguo exista, es decir, exista un fichero en la particion con ese nombre (existeFichero != -1) 
   //y que el nombre nuevo no exista (nombreDisponible == -1), es decir, no se encuentre ningun fichero en la particion con ese nombre


   if(existeFichero == -1){
      printf("\nERROR. Fichero %s no encontrado\n", nombreantiguo);
   }
   else if(nombreDisponible != -1){
      printf("\nERROR. El fichero %s ya existe\n", nombrenuevo);
   }
   else if(existeFichero != -1 && nombreDisponible == -1){
      renombrarPosible = 1;
      for(int i = 0; i < LEN_NFICH; i++){
         directorio[existeFichero].dir_nfich[i] = nombrenuevo[i];
      }
   }

   return renombrarPosible;

}

int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre){
   //Si encuentra el fichero devuelve su indice en el array directorio, si no devuelve -1

   int indexFichero = -1;

   for(int i = 0; i < MAX_FICHEROS; i++){

      if(strcmp(nombre, directorio[i].dir_nfich) == 0){
            indexFichero = i;
            i = MAX_FICHEROS; //para salir del bucle
      }
         
   }
   return indexFichero;

}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char* nombre){

   int existeFichero = BuscaFich(directorio, inodos, nombre);
   int indiceMemDatos, contTextDatos = 0;

   if(existeFichero != -1){
      printf("\n");

       for (int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++) {
            if (inodos->blq_inodos[directorio[existeFichero].dir_inodo].i_nbloque[i] == 0xFFFF) {
               continue;
            }
            else{
               contTextDatos = 0;
               indiceMemDatos = inodos->blq_inodos[directorio[existeFichero].dir_inodo].i_nbloque[i];

               for(int j = 0; j < SIZE_BLOQUE; j++){
                  printf("%c", memdatos[indiceMemDatos - 4].dato[j]);
               }
            }
       }
   }
   printf("\n");
}

int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre,  FILE *fich){
   int errorOrigen=0;
   int indiceInodoBorrar;
   if((indiceInodoBorrar=BuscaFich(directorio,inodos,nombre))==-1){
      printf("No existe el fichero origen, por favor, introduzca uno que exista en el directorio\n");
      Directorio(directorio,inodos);
      errorOrigen=1;
   }
   else{
      inodos->blq_inodos[directorio[indiceInodoBorrar].dir_inodo].size_fichero=0;
      strcpy(directorio[indiceInodoBorrar].dir_nfich," ");
      ext_bytemaps->bmap_inodos[directorio[indiceInodoBorrar].dir_inodo]=0;
      ext_superblock->s_free_inodes_count++; 
      for(int i=0;i<MAX_NUMS_BLOQUE_INODO;i++){
         if(ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio[indiceInodoBorrar].dir_inodo].i_nbloque[i]]){
            ext_bytemaps->bmap_bloques[inodos->blq_inodos[directorio[indiceInodoBorrar].dir_inodo].i_nbloque[i]]=0;
            ext_superblock->s_free_blocks_count++;
         }    
      }
      directorio[indiceInodoBorrar].dir_inodo=0xFFFF; 
   }   
   return errorOrigen;
}
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich){
   
   int errorOrigen=0; 
   int indiceBloqueOrigen;
   int indiceFicheroOrigen;
   int indiceBloqueDestino;
   int indiceFicheroDestino;
   int indiceInodoLibre=0;
   if((indiceFicheroOrigen=BuscaFich(directorio,inodos,nombreorigen))==-1){
      printf("No existe el fichero origen, por favor, introduzca uno que exista en el directorio\n");
      Directorio(directorio,inodos);
      errorOrigen=1;
   }
   else{
      if((indiceFicheroDestino = BuscaFich(directorio, inodos, nombredestino)) != -1){
         printf("\nPrecaucion, el fichero %s va a ser sobreescrito", nombredestino);

         indiceInodoLibre = directorio[indiceFicheroDestino].dir_inodo; //Asignamos a indice inodo libre el inodo del fichero que se va a sobreescribir. Por conveniencia usamos la variable
                                                                        // indiceInodoLibre, pero este inodo no esta libre
         for(int i = 0; i < MAX_NUMS_BLOQUE_INODO; i++){
            if(inodos->blq_inodos[indiceInodoLibre].i_nbloque[i] != 0xFFFF){
               ext_superblock->s_free_blocks_count++;
               ext_bytemaps->bmap_bloques[inodos->blq_inodos[indiceInodoLibre].i_nbloque[i]] = 0;
            }
            inodos->blq_inodos[indiceInodoLibre].i_nbloque[i] = 0xFFFF;
         }

      }
      else{

         for(int i = 4; i < MAX_INODOS; i++){
            if(ext_bytemaps->bmap_inodos[i] == 0){
               indiceInodoLibre = i;
               i = MAX_INODOS;
            }
         }  

         //QUE PASA SI TODOS LOS INODOS ESTAN OCUPADOS

         //SI HAY INODO LIBRE
         ext_superblock->s_free_inodes_count--;

         for(int i = 1; i < MAX_FICHEROS; i++){
         if(directorio[i].dir_inodo==0xFFFF){
            directorio[i].dir_inodo=indiceInodoLibre;
            strcpy(directorio[i].dir_nfich,nombredestino);
            indiceFicheroDestino = i;
            i=MAX_FICHEROS;
         }
      }

      }

      //QUE PASA SI TODOS LOS DIRECTORIOS ESTAN OCUPADOS
      ext_bytemaps->bmap_inodos[indiceInodoLibre]=1;
      inodos->blq_inodos[indiceInodoLibre].size_fichero=inodos->blq_inodos[directorio[indiceFicheroOrigen].dir_inodo].size_fichero;

      for (int j = 0; j < MAX_NUMS_BLOQUE_INODO; j++) {
         if (inodos->blq_inodos[directorio[indiceFicheroOrigen].dir_inodo].i_nbloque[j] == 0xFFFF) {
            continue;
         } 
         else {
            indiceBloqueOrigen = inodos->blq_inodos[directorio[indiceFicheroOrigen].dir_inodo].i_nbloque[j];
            for(int i=0;i<MAX_BLOQUES_PARTICION;i++){
               if(ext_bytemaps->bmap_bloques[i]==0){
                  indiceBloqueDestino=i;
                  i=MAX_BLOQUES_PARTICION;     
               }
            }

            //QUE PASA SI TODOS LOS BLOQUES ESTAN OCUPADOS

            //SI HAY BLOQUES LIBRES
            ext_superblock->s_free_blocks_count--;

            ext_bytemaps->bmap_bloques[indiceBloqueDestino]=1;
            inodos->blq_inodos[directorio[indiceFicheroDestino].dir_inodo].i_nbloque[j]=indiceBloqueDestino;
            memcpy(memdatos[indiceBloqueDestino - 4].dato, memdatos[indiceBloqueOrigen - 4].dato, SIZE_BLOQUE);

         }
      } 
   }

   return errorOrigen;

}

void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich) {
    fseek(fich, SIZE_BLOQUE * 3, SEEK_SET); 
    fwrite(directorio, SIZE_BLOQUE, 1, fich);

    fseek(fich, SIZE_BLOQUE * 2, SEEK_SET);
    fwrite(inodos, SIZE_BLOQUE, 1, fich);
}

void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich) {
    fseek(fich, SIZE_BLOQUE, SEEK_SET); 
    fwrite(ext_bytemaps, SIZE_BLOQUE, 1, fich);
}

void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich) {
    fseek(fich, 0, SEEK_SET); 
    fwrite(ext_superblock, SIZE_BLOQUE, 1, fich);
}

void GrabarDatos(EXT_DATOS *memdatos, FILE *fich) {
    fseek(fich, SIZE_BLOQUE * 4, SEEK_SET);
    fwrite(memdatos, SIZE_BLOQUE, MAX_BLOQUES_DATOS, fich);
    printf("Grabar datos terminado");
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
