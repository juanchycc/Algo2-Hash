#include"lista.h"

#include <stdbool.h>
#include <stddef.h>
#include<stdlib.h>
#include<stdio.h>

#define ERROR -1
#define EXITO 0
#define ERROR_BUSQUEDA 0

typedef struct nodo{
	void* elemento;
	struct nodo* siguiente;
}nodo_t;

typedef struct lista{
	nodo_t* nodo;
	size_t cantidad;
	lista_comparador comparador;
	lista_liberar_elemento destructor;
}lista;

typedef struct lista_iterador{
	nodo_t* nodo;
}lista_iterador;


void inicializar_nodo(nodo_t* nodo,void*elemento){
	nodo->elemento  = elemento;
	nodo->siguiente = NULL;
}

/*busca el ultimo nodo de la lista pero devuelve el ante ultimo en caso
 de que la cantidad sea mayor a uno*/
nodo_t* buscar_ultimo(lista_t* lista){
	if(!lista || lista->cantidad == 0)
		return NULL;

	if(lista->cantidad==1)
		return lista->nodo;

	nodo_t* nodo_aux = lista->nodo->siguiente;
	nodo_t* nodo_anterior = lista->nodo;

	bool encontrado = false;
	int i=2;

	while(!encontrado && i<=(lista->cantidad)){

		if(nodo_aux->siguiente==NULL){
			encontrado=true;
		}else{
			nodo_anterior = nodo_aux;
			nodo_aux = (nodo_aux->siguiente);
			i++;
		}
	}
	if(encontrado)
		return nodo_anterior;
	else
		return NULL;
}

/*devuelve el nodo de la posicion anterior a la pedida en caso
de que sea mayor a cero la posicion*/
nodo_t* nodo_posicion_anterior(lista_t* lista, size_t posicion){
	if(!lista)
		return NULL;

	if(posicion ==0)
		return lista->nodo;

	nodo_t* nodo_anterior = lista->nodo;
	nodo_t* nodo_posicion = nodo_anterior->siguiente;

	bool encontrado = false;
	int i=1;

	while( !encontrado && i<(lista->cantidad) ){

		if(  i == posicion )
			encontrado = true;
		else{
			nodo_anterior = nodo_posicion;
			nodo_posicion = nodo_posicion->siguiente;
			i++;
		}
	}
	if(encontrado)
		return nodo_anterior;
	else
		return NULL;
}



lista_t* lista_crear(lista_comparador comparador,lista_liberar_elemento destructor){
	
	lista_t* lista = calloc(1,sizeof(lista_t));
	
	if(lista!=NULL){
		lista->comparador = comparador;
		lista->destructor = destructor;
		lista->cantidad=0;
		lista->nodo=NULL;

	}else{
		return NULL;
	}
	return lista;
}



int lista_insertar(lista_t* lista, void* elemento){ 	
	if(lista==NULL){
		return ERROR;
	}

	if(lista->cantidad==0){

		lista->nodo = calloc(1,sizeof(nodo_t));
		
		if(lista->nodo==NULL)
			return ERROR;

		inicializar_nodo(lista->nodo,elemento);
		lista->cantidad++;
		return EXITO;
	}

	nodo_t* ultimo = buscar_ultimo(lista);

	if( ultimo == NULL)
		return ERROR;

	if( lista->cantidad > 1)
		ultimo = ultimo->siguiente;

	ultimo->siguiente = calloc(1,sizeof(nodo_t));

	if( ultimo->siguiente==NULL )
		return ERROR;
		
	inicializar_nodo(ultimo->siguiente,elemento);

	lista->cantidad++;
	return EXITO;
}




int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	if(!lista)
		return ERROR;
	
	if( posicion > (lista->cantidad) )
		posicion = (lista->cantidad);

	if( posicion == (lista->cantidad) )
		return lista_insertar(lista, elemento);
	
	nodo_t* nodo_aux = lista->nodo;

	if( posicion==0 ){

		nodo_t* nuevo_nodo = calloc(1,sizeof(nodo_t));
		if( nuevo_nodo==NULL)
			return ERROR;

		nuevo_nodo->siguiente = nodo_aux;
		lista->nodo = nuevo_nodo;

		nuevo_nodo->elemento=elemento;
		(lista->cantidad)++;
		return EXITO;
	}

	nodo_aux = nodo_posicion_anterior(lista, posicion);

	if( nodo_aux==NULL)
		return ERROR;

	nodo_t* nuevo_nodo = calloc(1,sizeof(nodo_t));

	if(nuevo_nodo == NULL)	    
		return ERROR;
		
	nuevo_nodo->siguiente = nodo_aux->siguiente;

	nodo_aux->siguiente = nuevo_nodo;

	nuevo_nodo->elemento = elemento;

	(lista->cantidad)++;

	return EXITO;

}



int lista_borrar(lista_t* lista){
	if(!lista)
		return ERROR;

	nodo_t* ante_ultimo = buscar_ultimo(lista);

	if( ante_ultimo == NULL)
		return ERROR;

	if( lista->cantidad > 1){
		lista->destructor(ante_ultimo->siguiente->elemento);
		free(ante_ultimo->siguiente);
		ante_ultimo->siguiente = NULL;

	}else{
		lista->destructor(ante_ultimo->elemento);
		free(ante_ultimo);
	}

	
	lista->cantidad -=1;

	return EXITO;
}



int lista_borrar_de_posicion(lista_t* lista, size_t posicion){

	if(!lista || lista->cantidad==0)
		return ERROR;

	if( posicion > ((lista->cantidad)-1) )
		posicion = (lista->cantidad)-1;

	if( posicion == (lista->cantidad)-1 )
		return lista_borrar(lista);

	nodo_t* nodo_aux = nodo_posicion_anterior(lista,posicion);

	if(nodo_aux == NULL)
		return ERROR;

	if( posicion == 0){

		lista->nodo = nodo_aux->siguiente;
		lista->destructor(nodo_aux->elemento);
		free(nodo_aux);

		lista->cantidad -=1;
		return EXITO;
	}

	nodo_t* nodo_posicion = nodo_aux->siguiente;

	nodo_aux->siguiente = nodo_posicion->siguiente;

	lista->destructor(nodo_posicion->elemento);
	free(nodo_posicion);
	lista->cantidad -= 1;

	return EXITO;
}




void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if(!lista || lista->cantidad==0 || !lista->nodo || (posicion > (lista->cantidad)-1) )
		return NULL;

	if( posicion == 0 ){
		return lista->nodo->elemento;
	}

	nodo_t* nodo_aux = nodo_posicion_anterior(lista,posicion);
	if( nodo_aux == NULL)
		return NULL;

	return (nodo_aux->siguiente->elemento);
}



void* lista_ultimo(lista_t* lista){
	if(!lista)
		return NULL;

	nodo_t* nodo_aux = buscar_ultimo(lista);

	if(nodo_aux == NULL)
		return NULL;
	if( lista->cantidad == 1)
		return (nodo_aux->elemento);

	return (nodo_aux->siguiente->elemento);

}



bool lista_vacia(lista_t* lista){
	if(lista==NULL)
		return true;

	if( lista->cantidad == 0)
		return true;

	else 
		return false;
}



size_t lista_elementos(lista_t* lista){
	if(!lista)
		return 0;

	return (lista->cantidad);
}



int lista_apilar(lista_t* lista, void* elemento){
	if(!lista)
		return ERROR;

	if(lista->cantidad==0){

		lista->nodo = malloc(sizeof(nodo_t));
		
		if(lista->nodo==NULL)
			return ERROR;

		inicializar_nodo(lista->nodo,elemento);
		lista->cantidad +=1;

		return EXITO;
	}

	nodo_t* nodo_aux = buscar_ultimo(lista);

	if(nodo_aux == NULL)
		return ERROR;

	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));

	if(!nodo_nuevo)
		return ERROR;

	if( lista->cantidad > 1){
		nodo_aux = nodo_aux->siguiente;
	}

	nodo_aux->siguiente = nodo_nuevo;
	inicializar_nodo(nodo_nuevo, elemento);
		
	lista->cantidad +=1;

	return EXITO;
}




int lista_desapilar(lista_t* lista){

	if(!lista)
		return ERROR;

	nodo_t* nodo_aux = buscar_ultimo(lista);

	if(nodo_aux == NULL)
		return ERROR;
	if( lista->cantidad > 1){
		free(nodo_aux->siguiente);
		nodo_aux->siguiente = NULL;
	}else{
		free(nodo_aux);
	}

	lista->cantidad -=1;

	return EXITO;

}



void* lista_tope(lista_t* lista){
	if(!lista)
		return NULL;

	nodo_t* nodo_aux = buscar_ultimo(lista);
	if(nodo_aux==0)
		return NULL;

	if( (lista->cantidad)>1)
		return (nodo_aux->siguiente->elemento);
	else
		return (nodo_aux->elemento);
}



int lista_encolar(lista_t* lista, void* elemento){
	if(!lista)
		return ERROR;

	if(lista->cantidad==0){

		lista->nodo = malloc(sizeof(nodo_t));
		
		if(lista->nodo==NULL)
			return ERROR;

		inicializar_nodo(lista->nodo,elemento);
		lista->cantidad +=1;

		return EXITO;
	}

	nodo_t* nodo_aux = buscar_ultimo(lista);

	if(nodo_aux == NULL)
		return ERROR;

	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));

	if(!nodo_nuevo)
		return ERROR;

	if( lista->cantidad > 1){
		nodo_aux = nodo_aux->siguiente;
	}

	nodo_aux->siguiente = nodo_nuevo;
	inicializar_nodo(nodo_nuevo, elemento);
		
	lista->cantidad +=1;

	return EXITO;

}



int lista_desencolar(lista_t* lista){
	if(!lista || lista->cantidad==0)
		return ERROR;

	nodo_t* nodo_aux = (lista->nodo->siguiente);

	free(lista->nodo);

	lista->nodo = nodo_aux;
	lista->cantidad -=1;

	return EXITO;
}



void* lista_primero(lista_t* lista){
	if(!lista || (lista->cantidad)==0)
		return NULL;

	return (lista->nodo->elemento); 
}



void lista_destruir(lista_t* lista){
	if(!lista )
		return;

	if( lista->cantidad == 0){
		free(lista);
		return;
	}

	if( lista->cantidad == 1 ){
		lista->destructor(lista->nodo->elemento);
		free(lista->nodo);
		free(lista);
		return;
	}
	
	nodo_t* nodo_aux = lista->nodo;
	nodo_t* nodo_siguiente = nodo_aux->siguiente;

	int i=1;
	while( i<=(lista->cantidad) ){
		lista->destructor(nodo_aux->elemento);
		free(nodo_aux);

		nodo_aux = nodo_siguiente;

		if( i < (lista->cantidad)-1 )
			nodo_siguiente = nodo_siguiente->siguiente;

		i++;
	}
	free(lista);
	return;
}



lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if(!lista)
		return NULL;

	lista_iterador_t* iterador = calloc(1,sizeof(lista_iterador_t));

	 if( iterador !=NULL){

	 	iterador->nodo = calloc(1,sizeof(nodo_t));
	 	if( iterador->nodo == NULL){
	 		free(iterador);
	 		return NULL;
	 	}
	 	iterador->nodo->siguiente = lista->nodo;
	}
	return iterador;
}



bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(!iterador)
		return false;

	bool siguiente = false;

	if( iterador->nodo->siguiente != NULL )
			siguiente = true;
	

	return siguiente;
}



void* lista_iterador_siguiente(lista_iterador_t* iterador){
	if(!lista_iterador_tiene_siguiente(iterador))
		return NULL;

	void* elemento = (iterador->nodo->siguiente->elemento);
	iterador->nodo->siguiente = iterador->nodo->siguiente->siguiente;

	return elemento;
}


void lista_iterador_destruir(lista_iterador_t* iterador){
	if(!iterador)
		return;
	free(iterador->nodo);
	free(iterador);
}


void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){

	if(!lista || lista->cantidad==0)
		return;

	void* elemento;

	for(size_t i=0;i<lista->cantidad;i++){

		elemento = lista_elemento_en_posicion(lista,i);
		funcion(elemento,contexto);
	}

}


int lista_buscar(lista_t* lista,void* elemento){
	if( !lista )
		return ERROR;

	size_t posicion = 0;
	int encontrado = -1;

	while(encontrado!=0 && posicion<lista->cantidad){
		encontrado = lista->comparador(lista_elemento_en_posicion(lista,posicion),elemento);
		if( encontrado != 0)
			posicion++;
	}
	if(encontrado==0)
		return (int)posicion;
	else
		return ERROR;
}
