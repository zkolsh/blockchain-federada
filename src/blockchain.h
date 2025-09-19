#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stddef.h>
#include <stdlib.h>

typedef struct Nodo {
	size_t anterior;
	size_t id;
	size_t long_mensaje;
	char* mensaje;
} NodoBC;

NodoBC* nodobc_crear();
void nodobc_destruir(NodoBC* nodo);

#endif //BLOCKCHAIN_H
