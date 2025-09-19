#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stddef.h>
#include <stdlib.h>

typedef struct NodoBC {
	struct NodoBC* anterior;
	size_t id;
	size_t long_mensaje;
	char* mensaje;
} NodoBC;

NodoBC* nodobc_crear();
void nodobc_destruir(NodoBC* nodo);

#endif //BLOCKCHAIN_H
