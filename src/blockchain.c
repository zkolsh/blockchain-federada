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

Blockchain* bl_agregar_inicio(Blockchain* cadena, size_t id, size_t long_mensaje, char* mensaje) {
	if (cadena == NULL) {
		return bl_crear(id, long_mensaje, mensaje);
	};

	Blockchain* n = bl_crear(id, long_mensaje, mensaje);
	n->siguiente = cadena;
	cadena->anterior = n;
	return n;
};
