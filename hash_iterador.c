#include"hash_iterador.h"
#include"lista.h"
#include<stdlib.h>
#include<string.h>
#include"hash.h"

#define ERROR_MEMORIA NULL

typedef struct elemento{
	void* elemento;
	char* clave;
}elemento_t;

typedef struct contenedor{
	lista_t* elementos;
}contenedor_t;

typedef struct hash{
	contenedor_t* contenedores;
	int ocupados;
	size_t capacidad;
	hash_destruir_dato_t destruir_elemento;
}hash;

typedef struct hash_iter{
	contenedor_t* contenedores;
	int posicion;
	size_t tope;
	size_t posicion_lista;
}hash_iterador;

hash_iterador_t* hash_iterador_crear(hash_t* hash){
	if(!hash)
		return ERROR_MEMORIA;

	hash_iterador_t* iterador = calloc(1,sizeof(hash_iterador_t));
	if(!iterador)
		return ERROR_MEMORIA;

	iterador->contenedores = hash->contenedores;
	iterador->tope = hash->capacidad;
	return iterador;
}

const char* hash_iterador_siguiente(hash_iterador_t* iterador){
	if( !iterador || iterador->posicion >= iterador->tope)
		return NULL;

	lista_t* lista = iterador->contenedores[iterador->posicion].elementos;

	size_t cantidad = lista_elementos(lista);
	elemento_t* elemento;

	if( cantidad > 1 ){

		elemento = lista_elemento_en_posicion(lista,iterador->posicion_lista);

		(iterador->posicion_lista)++;

		if(!elemento)
			return hash_iterador_siguiente(iterador);

		if( cantidad == iterador->posicion_lista){
			iterador->posicion_lista = 0;
			(iterador->posicion)++;
		}

	}else{

		elemento = lista_elemento_en_posicion(lista,0);
		
		(iterador->posicion)++;

		if(!elemento)
			return hash_iterador_siguiente(iterador);
		
	}

	return elemento->clave;
}

bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador || iterador->posicion >= iterador->tope)
		return false;


	if( iterador->posicion == (iterador->tope)-1){

		size_t cantidad = lista_elementos( iterador->contenedores[iterador->posicion].elementos);

		if( cantidad == iterador->posicion_lista)
			return false;	
	}

	return true;
}

void hash_iterador_destruir(hash_iterador_t* iterador){
	free(iterador);
}
