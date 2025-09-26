#ifndef MERKLE_H
#define MERKLE_H

#include <stdbool.h>
#ifndef MERKLE_CAPACIDAD_MINIMA
#define MERKLE_CAPACIDAD_MINIMA 16
#endif //MERKLE_CAPACIDAD_MINIMA

#include "generador_primos.h"
#include "blockchain.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Merkle {
	size_t capacidad_nodos;
	unsigned long long* nodos;
	size_t longitud_primos;
	size_t indice_primos;
	int* primos;
	size_t cantidad_bloques;
	size_t capacidad_bloques;
	Blockchain** bloques;
} Merkle;

Merkle* merkle_crear(size_t cant_nodos, Blockchain** nodos);
void merkle_destruir(Merkle* arbol);
void merkle_realloc_nodos(Merkle* arbol, size_t nueva_capacidad);
void merkle_realloc_bloques(Merkle* arbol, size_t nueva_capacidad);
size_t merkle_alta(Merkle* arbol, Blockchain* nodo);
void merkle_actualizar(Merkle* arbol, size_t id_blockchain, size_t id_nodo, size_t long_mensaje, char* mensaje);
void merkle_amendar(Merkle* arbol, size_t id_blockchain, size_t id_nodo, size_t long_mensaje, char* mensaje);
bool merkle_validar(Merkle* arbol);
bool merkle_validar_subconjunto(Merkle* arbol, unsigned long long producto_esperado, size_t inicio, size_t fin);

#endif //MERKLE_H
