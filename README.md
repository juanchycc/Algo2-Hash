# Algo2-
Enunciado
Se pide implementar un TDA Hash. Para ello se brindan las firmas de las funciones públicas a implementar y se
deja a criterio del alumno tanto la creación de las funciones privadas del TDA para el correcto funcionamiento del
Hash cumpliendo con las buenas prácticas de programación. El Hash a implementar debe ser abierto y cumplir con
los requisitos detallados en el .h .
Adicionalmente se pide la creación de un TDA iterador externo para el recorrido de las claves almacenadas en el
Hash.

TDA Hash:
En este caso toco implementar un hash, este es un tipo de dato abstracto, es decir, un tipo de dato definido por una estructura de datos y operaciones que consideramos indispensables para el funcionamiento de este.

El hash funciona de manera asociativa con claves y posiciones WHAT? si.. basicamente el hash funciona junto a una funcion de hasheo, esta al recibir una clave devuelve una posicion del hash, es decir, si yo "hasheo" una cierta clave varias veces siempre voy a obtener la misma posicion,esto hace que a pesar de tener miles de elementos podamos tener una busqueda con una complejidad de O(1) :O, pero, no todo son mariposas... existe la posibilidad de que al hashear claves distintas obtengamos la misma posicion (esto va a depender del tipo de funcion de hasheo lo ideal es que esto no ocurra) para la resolucion de esto exiten dos tipos de hashes, los hashes abiertos se insertan los elementos dentro de una lista en la posicion del hash correspondiente o los hashes cerrados que lo resuelven en el mismo vector de hash utilizando metodos como la zona de desborde o la exploracion.
Otra funcion que se utiliza para que las colisiones disminuyan es el rehasheo, basicamente se toma un limite de ocupados generalmente es el 70% y cuando se llega a este porcentaje se amplia el hash y se reinsertar los elementos, ya que, al ser mas grande la funcion de hasheo puede darte una nueva posicion, ahora al ser mas grande el hash hay menos chances de colisiones :).
Aclaraciones:
En el caso de mi hash mi funcion de hasheo hace esto: dada una clave suma cada caracter (obteniendo un numero) y calculo el modulo con el tamaño del hash,mi limite es:70% y llegado a este punto aumento siempre al doble el tamaño del hash.
Esta implementacion fue de un hash abierto por lo que utilice la lista implementada anteriormente, ya que las claves se podian repetir necesite crear la funcion lista_buscar que devuelve la posicion del elmento o -1 si no, para su funcionamiento fue necesario tambien crear una lista_comparador dandole en este caso una funcion que compara las claves, y por ultimo fue necesario el lista_liberador por que cada elemento de los nodos es una estructura que tiene un elemento y una clave, el liberador de lista se encarga de liberar la clave y la estructura elemento_t.

Linea de Compilación:
gcc pruebas.c hash_iterador.c hash.c lista.c probador.c -o pruebas -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

Linea de Ejecución:
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
