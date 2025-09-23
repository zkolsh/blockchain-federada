#include "blockchain.h"

size_t proximo_id = 0;

Blockchain* Blockchain_crear() {
	Blockchain* n = malloc(sizeof(Blockchain));
	n->id = proximo_id++;
	n->long_mensaje = 0;
	n->anterior = NULL;
	n->siguiente = NULL;
	n->mensaje = NULL;
	return n;
};

void Blockchain_destruir(Blockchain* nodo) {
	if (nodo == NULL) {
		return;
	};

	// free(nodo->mensaje);
	free(nodo);
};

