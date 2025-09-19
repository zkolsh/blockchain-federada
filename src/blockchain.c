#include "blockchain.h"

size_t proximo_id = 0;

NodoBC* nodobc_crear() {
	NodoBC* n = malloc(sizeof(NodoBC));
	n->id = proximo_id++;
	n->long_mensaje = 0;
	n->anterior = (proximo_id != 1)? proximo_id - 1 : 0;
	n->mensaje = NULL;
	return n;
};

void nodobc_destruir(NodoBC* nodo) {
	if (nodo == NULL) {
		return;
	};

	// free(nodo->mensaje);
	free(nodo);
};

// Merkle* merkle_crear(size_t cant_nodos, NodoBC* nodos) {
// 	Merkle* arbol = malloc(sizeof(Merkle));
// 	arbol->heap = heap_new(sizeof(unsigned long long), comparar_longlongs, GT);
// 	arbol->primos = primos((int)arbol->heap->capacity);
// 	arbol->nodos = nodos;
// 	arbol->cantidad_nodos = cant_nodos;
// 	return arbol;
// };
