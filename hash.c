#include"lista.h"
#include"hash.h"
#include<stdlib.h>
#include<string.h>

#define UMBRAL 70
#define ERROR_ITERADOR_CANTIDAD 0
#define ERROR_MEMORIA NULL
#define ERROR -1
#define EXITO 0

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


int lista_comparador_claves(void* elemento_1,void* elemento_2){
	return strcmp(((elemento_t*)elemento_1)->clave,((elemento_t*)elemento_2)->clave);
}


void lista_destructor(void* elemento){
	free(((elemento_t*)elemento)->clave);
    free(elemento);
}


//Calcula la posicion correspondiente en el hash segun la clave.
int hashear(const char* clave,size_t capacidad){
	if(!clave)
		return ERROR;

	int posicion = 0;
	size_t i=0;

	while( clave[i] != '\0' ){
		posicion += clave[i];
		i++;
	}

	posicion = (posicion % (int)(capacidad-1));

	if( posicion < 0)
		posicion = (posicion*-1);
	else if( posicion >= (capacidad-1))//por el modulo no deberia pasar.. pero por las dudas
		posicion = 0;
	
	return posicion;
}




//Mide el porcentaje de ocupados del hash
float ocupabilidad(size_t capacidad,int ocupados){
	return ( ((float)ocupados * 100)/(float)capacidad );
}



//Busca en el hash la clave recibida, actualiza el valor posicion
//devuelve el struct elemento o null
elemento_t* buscar_elemento(lista_t* elementos,int* posicion,const char* clave){
	if(!clave)
		return ERROR_MEMORIA;

	bool encontrado = false;
	elemento_t* elemento = NULL;

	while( !encontrado && (*posicion)<lista_elementos(elementos)){
		elemento = lista_elemento_en_posicion(elementos,((size_t)*posicion));

		if( strcmp(elemento->clave,clave)==0)
			encontrado = true;
		if(!encontrado)
			(*posicion)++;
	}

	if(encontrado)
		return elemento;
	else
		return ERROR_MEMORIA;
}



//Libera los contenedores
void destruir_contenedores(contenedor_t* contenedores,size_t cantidad){

	for(size_t i=0;i<cantidad;i++){
		if( contenedores[i].elementos)
			lista_destruir(contenedores[i].elementos);
	}
	free(contenedores);
}




/*
Duplica el tamaño del hash volviendo a hashear e insertar los elementos
devuelve 0 si pudo -1 si surgio un error
*/
int rehashear(hash_t* hash){

	contenedor_t* contenedores_aux = hash->contenedores;
	size_t capacidad_auxiliar = hash->capacidad;

	hash->capacidad += hash->capacidad;

	hash->contenedores = calloc(1,(long unsigned int)(hash->capacidad)*sizeof(contenedor_t));
	if( !hash->contenedores )
		return ERROR;

	hash->ocupados = 0;

	for(size_t i=0;i<capacidad_auxiliar;i++){

		size_t cantidad = lista_elementos(contenedores_aux[i].elementos);
		size_t j = 0;

		while( j < cantidad){
				
			elemento_t* elemento = lista_elemento_en_posicion(contenedores_aux[i].elementos,j);
			if(!elemento)
				return ERROR;
			
			hash_insertar(hash,elemento->clave,elemento->elemento);
			j++;	
		}

	}
	destruir_contenedores(contenedores_aux,capacidad_auxiliar);

	return EXITO;
}



/*
crea la memoria necesaria para un nuevo elemento y le pasa
la clave y elemento.
*/


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){

	hash_t* hash = calloc(1,sizeof(hash_t));
	if(!hash)
		return ERROR_MEMORIA;

	if( capacidad < 3)
		capacidad = 3;

	hash->contenedores = calloc(1,capacidad*sizeof(contenedor_t));
	if( !hash->contenedores ){
		free(hash);
		return ERROR_MEMORIA;
	}

	hash->capacidad = capacidad;
	hash->destruir_elemento = destruir_elemento;

	return hash;
}




int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if( !hash || !clave )
		return ERROR;

	elemento_t* elemento_nuevo = calloc(1,sizeof(elemento_t));
	if( !elemento_nuevo)
		return ERROR;

	elemento_nuevo->elemento = elemento;
	elemento_nuevo->clave = calloc(1,sizeof(char)*(strlen(clave)+1));
	if( !elemento_nuevo->clave){
		free(elemento_nuevo);
		return ERROR;
	}

	strcpy(elemento_nuevo->clave,clave);

	int posicion = hashear(clave,hash->capacidad);

	if( !(hash->contenedores[posicion].elementos) ){

		hash->contenedores[posicion].elementos = lista_crear(lista_comparador_claves,lista_destructor);
		if( !hash->contenedores[posicion].elementos){
			free(elemento_nuevo->clave);
			free(elemento_nuevo);
			return ERROR;
		} 

		if(lista_insertar(hash->contenedores[posicion].elementos,elemento_nuevo) == ERROR){
			free(elemento_nuevo->clave);
			free(elemento_nuevo);
			free(hash->contenedores[posicion].elementos);
			return ERROR;
		}

		(hash->ocupados)++;
		
	}else{

		int buscado = lista_buscar(hash->contenedores[posicion].elementos,elemento_nuevo);
		if(buscado == ERROR){
			if(lista_insertar(hash->contenedores[posicion].elementos,elemento_nuevo) == ERROR){
				free(elemento_nuevo->clave);
				free(elemento_nuevo);
				return ERROR;
			}
		if( lista_elementos(hash->contenedores[posicion].elementos)==0)
			hash->ocupados++;

		}else{
			free(elemento_nuevo->clave);
			free(elemento_nuevo);
			hash_quitar(hash,clave);
			
			return hash_insertar(hash,clave,elemento);
		}

	} 

	if( ocupabilidad(hash->capacidad,hash->ocupados) >= UMBRAL ){

		if( rehashear(hash)==ERROR){
			return ERROR;
		}
	}

	return EXITO;
}



int hash_quitar(hash_t* hash, const char* clave){
	if( !clave || !hash )
		return ERROR;

	int posicion = hashear(clave,hash->capacidad);

	int posicion_elemento = 0;

	elemento_t* elemento = buscar_elemento(hash->contenedores[posicion].elementos,&posicion_elemento,clave);
	

	if(!elemento)
		return ERROR;
	else{
		if( lista_elementos(hash->contenedores[posicion].elementos) == 1)
			(hash->ocupados)--;

		if(hash->destruir_elemento)
			hash->destruir_elemento(elemento->elemento);

		lista_borrar_de_posicion(hash->contenedores[posicion].elementos,(size_t)posicion_elemento);
	}

	return EXITO;
}




void* hash_obtener(hash_t* hash, const char* clave){
	if( !hash || !clave )
		return ERROR_MEMORIA;

	int posicion = hashear(clave,hash->capacidad);
	int posicion_elemento = 0;
	elemento_t* elemento = buscar_elemento(hash->contenedores[posicion].elementos,&posicion_elemento,clave);
	if(!elemento)
		return NULL;
	else
		return elemento->elemento;

}





bool hash_contiene(hash_t* hash, const char* clave){
	if( !clave || !hash )
		return false;

	if( hash_obtener(hash,clave))
		return true;

	return false;
}




size_t hash_cantidad(hash_t* hash){
	if(!hash)
		return ERROR_ITERADOR_CANTIDAD;

	size_t cantidad = 0;

	for(size_t i=0;i<hash->capacidad;i++){

		lista_t* elementos = hash->contenedores[i].elementos;

		if( elementos )
			cantidad += lista_elementos(elementos);
		
	}

	return cantidad;
}





void hash_destruir(hash_t* hash){
	if(!hash)
		return;

	for(size_t i=0;i<hash->capacidad;i++){

		lista_t* elementos = hash->contenedores[i].elementos;

		if( elementos ){

			for(size_t j=0;j<lista_elementos(elementos);j++){
				
				elemento_t* elemento = lista_elemento_en_posicion(elementos,j);	
				
				if(hash->destruir_elemento)
					hash->destruir_elemento(elemento->elemento);
			}

			lista_destruir(elementos);
		}
	}

	free(hash->contenedores);
	free(hash);
}






size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if( !hash  )
		return ERROR_ITERADOR_CANTIDAD;

	bool parar = false;
	size_t contador = 0;
	size_t cantidad_llamados = 0;

	while( !parar && (contador < hash->capacidad) ){

		lista_t* elementos =  hash->contenedores[contador].elementos;
		size_t cantidad = lista_elementos(elementos);

		size_t j = 0;

		while( (j < cantidad) && !parar ){
			elemento_t* elemento_aux = lista_elemento_en_posicion(elementos,j);

			if(funcion)
				parar = funcion(hash,elemento_aux->clave,aux);
			j++;
			cantidad_llamados++;
		}

	contador++;

	}

	return cantidad_llamados;
}

