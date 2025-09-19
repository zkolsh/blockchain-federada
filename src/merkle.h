#ifndef MERKLE_H
#define MERKLE_H

#ifndef MERKLE_CAPACIDAD_MINIMA
#define MERKLE_CAPACIDAD_MINIMA 16
#endif //MERKLE_CAPACIDAD_MINIMA

#include "generador_primos.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct NodoBC NodoBC;

typedef struct Merkle {
	size_t capacidad;
	size_t tamaño;
	unsigned long long* datos;
	int* primos;
	// NodoBC* nodos;
} Merkle;

Merkle* merkle_crear(size_t cant_nodos, NodoBC* nodos);
void merkle_destruir(Merkle* arbol);
void merkle_alta(Merkle* arbol, NodoBC* nodo);
void merkle_actualizar(Merkle* arbol, size_t id, size_t long_mensaje, char* mensaje);
void merkle_validar(Merkle* arbol);
void merkle_validar_subconjunto(Merkle* arbol, size_t a, size_t b);

#endif //MERKLE_H
