#ifndef _PROBADOR_H
#define _PROBADOR_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct probador probador_t;


probador_t* crear_probador();

//void avisar_prueba(probador_t* probador,char*aviso);

void describir_prueba(probador_t* probador,char* descripcion,bool paso_prueba);

void estadisticas_prueba(probador_t* probador);

void destruir_probador(probador_t* probador);

#endif /*_PROBADOR_H*/
