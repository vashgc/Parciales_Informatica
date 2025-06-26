#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "audio_player.h"
#include <string.h>


//PARCIAL DE ULISES VALENTIN GRIGOR


int main(){

    FILE *f, *fpistas;
    f = fopen("audio_list.raw","rb");
    if(f==NULL){
        perror("");
        exit(-1);
    }
    //variables
    uint32_t cant_pistas;
    fread(&cant_pistas,sizeof(uint32_t),1,f);
    printf("Hay %d pistas en el archivo\n",cant_pistas);
    uint32_t sanple_rate[cant_pistas], sanple_count[cant_pistas], aux;
    char auxnom[64];
    
    //seccion donde guardo los valores que se van leyendo
    printf("\n-------nombres de canciones:-------\n");
    for(int i=0;i<cant_pistas;i++){
        fread(&aux,sizeof(uint32_t),1,f);
        sanple_rate[i]=aux;
        //printf("%dHz\n",aux);
        fread(&aux,sizeof(uint32_t),1,f);
        sanple_count[i]=aux;
       // printf("%d muestras\n",aux);
        fread(auxnom,sizeof(char),64,f);
        printf("[%d]-%s\n",i+1,auxnom);  //printeo los nombres de las canciones

    }
    
    printf("\n");
    printf("---------------------------------");
    printf("\n");

    //declaro los vectores que se van a usar en la funcion play_audio
  float **canciones = malloc(cant_pistas*sizeof(float *)), *auxx;
  for(int i=0;i<cant_pistas;i++){
    auxx = malloc(sanple_count[i]*sizeof(float));
    fread(auxx,sizeof(float),sanple_count[i],f);
    canciones[i]=malloc(sanple_count[i]*sizeof(float));
    canciones[i]=auxx;
  }
  int seleccion, bandera=0;


    //Pseudo-Menu donde le doy 5 opciones al usuario
  do{
    printf("Que cancion quiere reproducir? Hay %d opciones. Si quiere salir presione 5 ", cant_pistas);
    scanf("%d",&seleccion);
    while(seleccion>5 || seleccion<=0){
        printf("\nseleccione una opcion valida: \n");
        scanf("%d",&seleccion);
    }
       
        //printf("\n1");
        if(seleccion<=4){
             seleccion--; //porque va desde 0-3, si el usuario pone 4 se rompe
            play_audio(sanple_rate[seleccion],sanple_count[seleccion],canciones[seleccion]);
            }
        if(seleccion==5){
            bandera=1;
        }
    }while(bandera==0);
   // printf("\n1");

    
  fclose(f);    //cierro el anterior archivo
  
  f = fopen("sound.raw","w");   //abro un archivo en forma de escritura
    if(f==NULL){
        perror("");
        exit(-1);
    }

   printf("Que cancion quiere guardar en el archivo? Hay %d opciones. ", cant_pistas);
    scanf("%d",&seleccion);
    while(seleccion>4 || seleccion<=0){
        printf("\nseleccione una opcion valida: \n");
        scanf("%d",&seleccion);
    }

  seleccion--;
fwrite(canciones[seleccion],sizeof(float)*sanple_count[seleccion],1,f);

//libero los vectores y cierro el archivo

    fclose(f);
    free(auxx);
    for(int i=0;i<cant_pistas;i++){
  free(canciones[i]);
  }
    return 0;
}