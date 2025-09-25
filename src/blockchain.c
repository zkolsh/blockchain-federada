#include "blockchain.h"

Blockchain* bl_crear(size_t id, size_t long_mensaje, char* mensaje) {
	Blockchain* n = malloc(sizeof(Blockchain));
	n->anterior = n->siguiente = NULL;
	n->id = id;
	n->long_mensaje = long_mensaje;
	n->mensaje = mensaje;
	return n;
};

void bl_destruir(Blockchain* nodo) {
	if (nodo == NULL) {
		return;
	};

	bl_destruir(nodo->siguiente);
	free(nodo);
};

Blockchain* bl_agregar_final(Blockchain* cadena, size_t id, size_t long_mensaje, char* mensaje) {
	if (cadena == NULL) {
		return bl_crear(id, long_mensaje, mensaje);
	};

	Blockchain* ultimo = cadena;
	while (ultimo->siguiente) {
		ultimo = ultimo->siguiente;
	};

	Blockchain* n = bl_crear(id, long_mensaje, mensaje);
	n->siguiente = NULL;
	n->anterior = ultimo;
	ultimo->siguiente = n;

	return cadena;
};
