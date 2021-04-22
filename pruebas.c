#include"probador.h"
#include"colores.h"
#include"hash_iterador.h"
#include<stdio.h>
#include "lista.h"
#include<string.h>

#define MAX_DESCRIPCION 60
#define ERROR -1
#define EXITO 0
#define MAX_CLAVES 100
#define CANTIDAD 6

void destructor(void* elemento){
	return;
}

 bool imprimir_elementos(hash_t* hash, const char* clave, void* aux){
 	if( !clave || !hash)
 		return true;

 	void* elemento = hash_obtener(hash,clave);


 	printf("elemento de la clave %s:%li\n",clave,(long)elemento);
 	return false;
 }



void probar_insertar_en_hash(hash_t* hash,char clave[],long elemento,probador_t* probador){
	
	char descripcion[MAX_DESCRIPCION]="Se inserta elemento en el hash";
	bool error = false;

	if(hash_insertar(hash,clave,(void*)(elemento)) == ERROR)
		error = true;
	

	if( !error)
		describir_prueba(probador,descripcion,true);
	else
		describir_prueba(probador,descripcion,false);
	
	return;
}


void probar_hash_contiene(hash_t* hash,char clave[],probador_t* probador){
	
	char descripcion[MAX_DESCRIPCION]="Chequeamos si contiene al elemento";


	if( hash_contiene(hash,clave))
		describir_prueba(probador,descripcion,true);
	else
		describir_prueba(probador,descripcion,false);
	
	return;
}


void probar_hash_obtener(hash_t* hash,char clave[],probador_t* probador){
	
	char descripcion[MAX_DESCRIPCION]="Se prueba obtener los elementos.";
	bool error = false;
	
	void* elemento = hash_obtener(hash,clave);
	if(!elemento )
		error = true;

	printf("\tclave:%s elemento:%li\n",clave,(long)elemento);
	
	
	if( !error)
		describir_prueba(probador,descripcion,true);
	else
		describir_prueba(probador,descripcion,false);
	
	return;
}

bool probar_hash_quitar(hash_t* hash){
	int retorno = hash_quitar(hash,"tdahash");

	if(retorno == EXITO && !hash_contiene(hash,"tdahash"))
		return true;
	else
		return false;
}

bool probar_hash_cantidad(hash_t* hash){
	size_t retorno = hash_cantidad(hash);

	if(retorno == CANTIDAD)
		return true;
	else
		return false;
}	

void probar(hash_t* hash,probador_t* probador,bool (*prueba)(hash_t*),char descripcion[MAX_DESCRIPCION]){
	bool paso = prueba(hash);

	describir_prueba(probador,descripcion,paso);
}


void probar_iterador_externo(hash_t* hash,probador_t* probador){
	printf(COLOR_AMARILLO"Pruebas de iterador externo:\n"COLOR_NORMAL);
	bool paso = false;

	hash_iterador_t* iterador = hash_iterador_crear(hash);
	if( iterador)
		paso = true;

	describir_prueba(probador,"Se crea el iterador externo",paso);

  	while(hash_iterador_tiene_siguiente(iterador)){
   		const char* clave = hash_iterador_siguiente(iterador);
     	printf(COLOR_AZUL "clave: %s elemento: %li\n", clave,(long)hash_obtener(hash,clave));
    }

	hash_iterador_destruir(iterador);
}

void insertar_muchos(hash_t* hash){
	hash_insertar(hash,"al",(void*)1);
	hash_insertar(hash,"go",(void*)2);
	hash_insertar(hash,"ri",(void*)3);
	hash_insertar(hash,"tm",(void*)4);
	hash_insertar(hash,"oS",(void*)5);
	hash_insertar(hash,"me",(void*)6);
	hash_insertar(hash,"nd",(void*)7);
	hash_insertar(hash,"ez",(void*)8);
	hash_insertar(hash,"dos",(void*)9);
	hash_insertar(hash,"tda",(void*)10);
	hash_insertar(hash,"ha",(void*)11);
	hash_insertar(hash,"sh",(void*)12);
	hash_insertar(hash,"in",(void*)13);
	hash_insertar(hash,"ser",(void*)14);
	hash_insertar(hash,"to",(void*)15);
	hash_insertar(hash,"mu",(void*)16);
	hash_insertar(hash,"ch",(void*)17);
	hash_insertar(hash,"os",(void*)18);
	hash_insertar(hash,"el",(void*)19);
	hash_insertar(hash,"em",(void*)111);
	hash_insertar(hash,"en",(void*)122);
	hash_insertar(hash,"tos",(void*)133);
	hash_insertar(hash,"enc",(void*)144);
	hash_insertar(hash,"ua",(void*)155);
	hash_insertar(hash,"ren",(void*)166);
	hash_insertar(hash,"tena",(void*)177);

	hash_insertar(hash,"AL",(void*)1);
	hash_insertar(hash,"GO",(void*)2);
	hash_insertar(hash,"RI",(void*)3);
	hash_insertar(hash,"TM",(void*)4);
	hash_insertar(hash,"Os",(void*)5);
	hash_insertar(hash,"ME",(void*)6);
	hash_insertar(hash,"ND",(void*)7);
	hash_insertar(hash,"EZ",(void*)8);
	hash_insertar(hash,"DOS",(void*)9);
	hash_insertar(hash,"TDA",(void*)10);
	hash_insertar(hash,"HA",(void*)11);
	hash_insertar(hash,"SH",(void*)12);
	hash_insertar(hash,"IN",(void*)13);
	hash_insertar(hash,"SER",(void*)14);
	hash_insertar(hash,"TO",(void*)15);
	hash_insertar(hash,"MU",(void*)16);
	hash_insertar(hash,"CH",(void*)17);
	hash_insertar(hash,"OS",(void*)18);
	hash_insertar(hash,"EL",(void*)19);
	hash_insertar(hash,"EM",(void*)111);
	hash_insertar(hash,"EN",(void*)122);
	hash_insertar(hash,"TOS",(void*)133);
	hash_insertar(hash,"ENC",(void*)144);
	hash_insertar(hash,"UA",(void*)155);
	hash_insertar(hash,"REN",(void*)166);
	hash_insertar(hash,"TENA",(void*)177);

	hash_insertar(hash,"TOS",(void*)133);
	hash_insertar(hash,"ENC",(void*)144);
	hash_insertar(hash,"UA",(void*)155);
	hash_insertar(hash,"REN",(void*)166);
	hash_insertar(hash,"TENA",(void*)177);

	hash_quitar(hash,"TENA");
	hash_quitar(hash,"AL");
	hash_quitar(hash,"EL");

	printf("%li\n",hash_cantidad(hash));
	size_t iteraciones = hash_con_cada_clave(hash,NULL,NULL);
	if(iteraciones == hash_cantidad(hash))
		printf(COLOR_NORMAL"Cantidad de iteraciones correcta"COLOR_VERDE" TRUE\n");
	else
		printf(COLOR_NORMAL"Cantidad de iteraciones correcta"COLOR_ROJO" FALSE");
}


void correr_pruebas(hash_t* hash,probador_t* probador){
	
	printf(COLOR_AMARILLO"\nSe insertan  elementos al hash:\n"COLOR_NORMAL);
	probar_insertar_en_hash(hash,"ALGORITMOS",12,probador);
	probar_insertar_en_hash(hash,"tdahash",27,probador);
	probar_insertar_en_hash(hash,"aguante",32,probador);
	probar_insertar_en_hash(hash,"BOQUITA",100,probador);
	printf("\n");

	probar_hash_contiene(hash,"ALGORITMOS",probador);

	probar_iterador_externo(hash,probador);

	printf(COLOR_AMARILLO"\nSe inserta elemento con clave repetida\n"COLOR_NORMAL);
	probar_insertar_en_hash(hash,"BOQUITA",101,probador);
	probar_hash_contiene(hash,"BOQUITA",probador);

	printf(COLOR_AMARILLO"\nSe imprimen los elementos del hash con el iterador interno:\n"COLOR_AZUL);
	size_t iteraciones = hash_con_cada_clave(hash,imprimir_elementos,NULL);
	if(iteraciones == hash_cantidad(hash))
		printf(COLOR_NORMAL"Cantidad de iteraciones correcta"COLOR_VERDE" TRUE\n");
	else
		printf(COLOR_NORMAL"Cantidad de iteraciones correcta"COLOR_ROJO" FALSE");

	printf(COLOR_AMARILLO"\nSe insertan mas elementos y se pasa el umbral\n"COLOR_NORMAL);
	hash_insertar(hash,"rehashear",(void*)1);
	hash_insertar(hash,"JUAN",(void*)1);
	probar_insertar_en_hash(hash,"CPicO",7,probador);

	probar_hash_contiene(hash,"CPicO",probador);

	printf(COLOR_AMARILLO"\nObtener elementos validos: \n"COLOR_NORMAL);
	probar_hash_obtener(hash,"ALGORITMOS",probador);
	probar_hash_obtener(hash,"rehashear",probador);

	printf(COLOR_AMARILLO"\nObtener elemento invalido" COLOR_ROJO"(tiene que fallar)\n"COLOR_NORMAL);
	probar_hash_obtener(hash,"algoritmos",probador);
	printf("\n");

	probar(hash,probador,&(probar_hash_quitar),"Se elimina un elemento del hash");

	probar(hash,probador,&(probar_hash_cantidad),"Probamos que la cantidad de elementos sea la correcta");
	printf("\n");

	printf(COLOR_AMARILLO"Se imprimen los elementos del hash con el iterador interno\n"COLOR_AZUL);
	iteraciones = hash_con_cada_clave(hash,imprimir_elementos,NULL);
	if(iteraciones == hash_cantidad(hash))
		printf(COLOR_NORMAL"Cantidad de iteraciones correcta"COLOR_VERDE" TRUE");
	else
		printf(COLOR_NORMAL"Cantidad de iteraciones correcta"COLOR_ROJO" FALSE\n");

	printf("\n"COLOR_NORMAL);	

	insertar_muchos(hash);
}


int main(){
	hash_t* hash = hash_crear(destructor,6);
	if( !hash){
		printf(COLOR_ROJO"Fallo al crear el hash");
		return -1;
	}
	probador_t* probador = crear_probador();
	
	correr_pruebas(hash,probador);

	destruir_probador(probador);
	hash_destruir(hash);

	return 0;
}
