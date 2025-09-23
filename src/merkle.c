#include "merkle.h"

Merkle* merkle_crear(size_t cant_nodos, Blockchain** nodos) {
	Merkle* arbol = malloc(sizeof(Merkle));
	arbol->capacidad = MERKLE_CAPACIDAD_MINIMA;
	arbol->tamaño = cant_nodos;
	arbol->datos = malloc(arbol->capacidad * sizeof(unsigned long long));
	arbol->primos = primos((int)arbol->capacidad);
	arbol->nodos = nodos;
	arbol->cantidad_nodos = cant_nodos;
	return arbol;
};

void merkle_destruir(Merkle* arbol) {
	if (!arbol) return;
	if (arbol->primos) free(arbol->primos);
	if (arbol->datos) free(arbol->datos);
	free(arbol);
};

void merkle_realloc(Merkle* arbol, size_t nueva_capacidad) {
	if (!arbol) return;
	if (nueva_capacidad <= arbol->capacidad) return;

	unsigned long long* nuevos_datos = realloc(arbol->datos, sizeof(unsigned long long) * nueva_capacidad);
	if (!nuevos_datos) {
		perror("no se pudo hacer realloc\n");
		exit(1);
	};

	arbol->datos = nuevos_datos;
	arbol->capacidad = nueva_capacidad;
};

size_t merkle_alta(Merkle* arbol, Blockchain *nodo) {
	if (!arbol) return 0;
	if (!nodo) return 0;

	if (arbol->tamaño >= arbol->capacidad) {
		merkle_realloc(arbol, arbol->capacidad * 2);
	};

	if (arbol->indice_primos >= arbol->longitud_primos) {
		free(arbol->primos);
		arbol->longitud_primos *= 2;
		arbol->primos = primos((int)arbol->longitud_primos);
	};

	arbol->datos[arbol->tamaño + 1] = arbol->primos[arbol->indice_primos++];
	size_t i = (arbol->tamaño + 1) / 2;

	arbol->tamaño++;
	arbol->cantidad_nodos++;
	while (i != 1) {
		size_t idx_izq = i * 2;
		size_t idx_der = i * 2 + 1;

		unsigned long long izq = (idx_izq > arbol->tamaño)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > arbol->tamaño)? 1 : arbol->datos[idx_der];
		arbol->datos[i] = izq * der;
		i /= 2;
	};

	return arbol->tamaño - (arbol->capacidad / 2);
};

void merkle_actualizar(Merkle* arbol, size_t id_blockchain, size_t id_nodo, size_t long_mensaje, char* mensaje) {
	if(arbol == NULL || id_blockchain > arbol->tamaño)
		return;

	Blockchain* indice_bl = arbol->nodos[id_blockchain];
	while(id_nodo != indice_bl->id && indice_bl != NULL) {
		indice_bl = indice_bl->siguiente;
	}

	if (indice_bl == NULL) {
		return;
	};

	free(indice_bl->mensaje);
	indice_bl->mensaje = mensaje;
	indice_bl->long_mensaje = long_mensaje;

	if (arbol->indice_primos >= arbol->longitud_primos) {
		free(arbol->primos);
		arbol->longitud_primos *= 2;
		arbol->primos = primos((int)arbol->longitud_primos);
	};

	size_t indice_mk = (arbol->capacidad / 2) + id_nodo;
	arbol->datos[indice_mk] = arbol->primos[arbol->indice_primos++];
	indice_mk /= 2;

	while (indice_mk != 1) {
		size_t idx_izq = indice_mk * 2;
		size_t idx_der = indice_mk * 2 + 1;

		unsigned long long izq = (idx_izq > arbol->tamaño)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > arbol->tamaño)? 1 : arbol->datos[idx_der];
		arbol->datos[indice_mk] = izq * der;
		indice_mk /= 2;
	};
}

void merkle_amendar(Merkle* arbol, size_t id_blockchain, size_t id_nodo, size_t long_mensaje, char* mensaje) {
	if(arbol == NULL || id_blockchain > arbol->tamaño)
		return;

	Blockchain* indice_bl = arbol->nodos[id_blockchain];
	indice_bl = bl_agregar_inicio(indice_bl, id_nodo, long_mensaje, mensaje);

	if (arbol->indice_primos >= arbol->longitud_primos) {
		free(arbol->primos);
		arbol->longitud_primos *= 2;
		arbol->primos = primos((int)arbol->longitud_primos);
	};

	size_t indice_mk = (arbol->capacidad / 2) + id_nodo;
	arbol->datos[indice_mk] = arbol->primos[arbol->indice_primos++];
	indice_mk /= 2;

	while (indice_mk != 1) {
		size_t idx_izq = indice_mk * 2;
		size_t idx_der = indice_mk * 2 + 1;

		unsigned long long izq = (idx_izq > arbol->tamaño)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > arbol->tamaño)? 1 : arbol->datos[idx_der];
		arbol->datos[indice_mk] = izq * der;
		indice_mk /= 2;
	};
};

unsigned long long merkle_validar_subarbol(Merkle* arbol, size_t id_nodo) {
	if (!arbol) return 0;
	if (id_nodo > arbol->capacidad) return 0;
	if (id_nodo > arbol->tamaño) return 1;
	if (id_nodo > arbol->capacidad / 2) return arbol->datos[id_nodo];
	unsigned long long izq = merkle_validar_subarbol(arbol, id_nodo * 2);
	unsigned long long der = merkle_validar_subarbol(arbol, id_nodo * 2 + 1);
	unsigned long long valor_esperado = izq * der;
	if (valor_esperado != arbol->datos[id_nodo]) {
		return 0;
	};

	return valor_esperado;
};

bool merkle_validar(Merkle* arbol) {
	if (!arbol) return false;
	for(size_t i=0; i<arbol->cantidad_nodos; i++){
		Blockchain* lista = arbol->nodos[i];
		if(lista == NULL) continue;
		if(lista->siguiente == NULL) continue;
		lista = lista->siguiente;
		while(lista != NULL){
			if(lista->id <= lista->anterior->id)
				return false;
		}
	}

	return merkle_validar_subarbol(arbol, 1);
};

bool merkle_validar_subconjunto(Merkle* arbol, size_t producto_esperado, size_t inicio, size_t fin) {
	if (!arbol) return false;
	unsigned long long producto = 1;

	for(size_t i=inicio; i<fin; i++){
		producto *= arbol->datos[(arbol->capacidad / 2) + i];

		Blockchain* lista = arbol->nodos[i];
		if(lista == NULL) continue;
		if(lista->siguiente == NULL) continue;
		lista = lista->siguiente;
		while(lista != NULL){
			if(lista->id <= lista->anterior->id)
				return false;
		}
	}

	return producto == producto_esperado;
};
