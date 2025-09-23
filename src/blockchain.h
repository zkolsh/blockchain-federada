#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stddef.h>
#include <stdlib.h>

typedef struct Blockchain {
	struct Blockchain* anterior;
	struct Blockchain* siguiente;
	size_t id;
	size_t long_mensaje;
	char* mensaje;
} Blockchain;

Blockchain* bl_crear();
void bl_destruir(Blockchain* nodo);
Blockchain* bl_agregar_inicio(Blockchain* cadena, size_t id, size_t long_mensaje, char* mensaje);

#endif //BLOCKCHAIN_H
