#include"probador.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define COLOR_VERDE "\x1b[32m\x1b[1m"
#define COLOR_ROJO "\x1b[31m\x1b[1m"
#define FONDO_VERDE "\x1b[42m\x1b[1m"
#define FONDO_ROJO "\x1b[41m\x1b[1m"
#define FONDO_NORMAL "\x1b[0m"

typedef struct probador{
	int cantidad_exitosas;
	int cantidad_pruebas;
}probador;



probador_t* crear_probador(){
	return calloc(1,sizeof(probador_t));
}


/*void avisar_prueba(probador_t* probador,char*aviso){

}*/

void describir_prueba(probador_t* probador,char* descripcion,bool paso_prueba){
	if(probador==NULL)
		return;

	if(paso_prueba)
		probador->cantidad_exitosas++;

	probador->cantidad_pruebas++;
	if(paso_prueba){
		printf("PRUEBA: %s"COLOR_VERDE" %s\n",descripcion,"EXITO"FONDO_NORMAL);
	}else{
		printf("PRUEBA: %s"COLOR_ROJO" %s\n",descripcion,"FALLO"FONDO_NORMAL);
	}
	
}

void estadisticas_prueba(probador_t* probador){
	if(!probador)
		return;
	int exito=(probador->cantidad_exitosas);
	int pruebas=(probador->cantidad_pruebas);
	if(exito==pruebas){
		printf("Pruebas exitosas" FONDO_VERDE" %i de %i\n"FONDO_NORMAL,probador->cantidad_exitosas,probador->cantidad_pruebas);
	}else{
		printf("Pruebas exitosas" FONDO_ROJO" %i de %i\n"FONDO_NORMAL,probador->cantidad_exitosas,probador->cantidad_pruebas);
	}
	
}

void destruir_probador(probador_t* probador){
	free(probador);
}


