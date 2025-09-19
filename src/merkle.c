#include "merkle.h"
#include "blockchain.h"

Merkle* merkle_crear(size_t cant_nodos, NodoBC* nodos) {
	Merkle* arbol = malloc(sizeof(Merkle));
	arbol->capacidad = MERKLE_CAPACIDAD_MINIMA;
	arbol->tamaño = cant_nodos;
	arbol->datos = malloc(arbol->capacidad * sizeof(unsigned long long));
	arbol->primos = primos((int)arbol->capacidad);
	// arbol->nodos = nodos;
	return arbol;
};

void merkle_destruir(Merkle* arbol) {
	if (!arbol) return;
	if (arbol->primos) free(arbol->primos);
	if (arbol->datos) free(arbol->datos);
	free(arbol);
};

void merkle_alta(Merkle* arbol, NodoBC *nodo) {
	if (!arbol) return;
	if (!nodo) return;

	arbol->datos[arbol->tamaño + 1] = arbol->primos[nodo->id];
	size_t i = (arbol->tamaño + 1) / 2;

	arbol->tamaño++;
	while (i != 1) {
		size_t idx_izq = i * 2;
		size_t idx_der = i * 2 + 1;

		unsigned long long izq = (idx_izq > arbol->tamaño)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > arbol->tamaño)? 1 : arbol->datos[idx_der];
		arbol->datos[i] = izq * der;
		i /= 2;
	};
};
