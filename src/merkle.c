#include "merkle.h"

Merkle* merkle_crear(size_t cant_nodos, Blockchain** nodos) {
	Merkle* arbol = malloc(sizeof(Merkle));
	arbol->capacidad_datos = MERKLE_CAPACIDAD_MINIMA;
	arbol->longitud_primos = arbol->capacidad_datos;
	arbol->indice_primos = 0;
	arbol->primos = primos((int)arbol->capacidad_datos);
	arbol->datos = malloc(arbol->capacidad_datos * sizeof(unsigned long long));
	for (size_t i = 0; i < arbol->capacidad_datos; i++) {
		arbol->datos[i] = 1ull;
	};

	arbol->cantidad_nodos = cant_nodos;
	arbol->capacidad_nodos = cant_nodos;
	if (arbol->capacidad_nodos < MERKLE_CAPACIDAD_MINIMA) {
		arbol->capacidad_nodos = MERKLE_CAPACIDAD_MINIMA;
	};

	arbol->nodos = (Blockchain**)malloc(arbol->capacidad_nodos * sizeof(Blockchain*));
	if (nodos) {
		memcpy((void*)arbol->nodos, (void*)nodos, cant_nodos * sizeof(Blockchain*));
	};

	return arbol;
};

void merkle_destruir(Merkle* arbol) {
	if (!arbol) return;
	if (arbol->primos) free(arbol->primos);
	if (arbol->datos) free(arbol->datos);
	if (arbol->nodos) {
		for (size_t i = 0; i < arbol->cantidad_nodos; i++) {
			if (!arbol->nodos[i]) continue;
			bl_destruir(arbol->nodos[i]);
		};

		free((void*)arbol->nodos);
	};
	free(arbol);
};

void merkle_realloc_datos(Merkle* arbol, size_t nueva_capacidad) {
	if (!arbol) return;
	if (nueva_capacidad <= arbol->capacidad_datos) return;

	unsigned long long* nuevos_datos = realloc(arbol->datos, sizeof(unsigned long long) * nueva_capacidad);
	if (!nuevos_datos) {
		perror("no se pudo hacer realloc\n");
		exit(1);
	};

	arbol->datos = nuevos_datos;
	size_t viejo_inicio = arbol->capacidad_datos / 2;
	size_t nuevo_inicio = nueva_capacidad / 2;
	size_t hojas_usadas = arbol->cantidad_nodos;

	if (hojas_usadas == 0) {
		arbol->capacidad_datos = nueva_capacidad;
		return;
	};

	size_t ultima_hoja = nuevo_inicio + hojas_usadas + 1;
	memmove(&nuevos_datos[nuevo_inicio], &arbol->datos[viejo_inicio],
		hojas_usadas * sizeof(unsigned long long));

	for (size_t i = nuevo_inicio + hojas_usadas; i < nueva_capacidad; i++) {
		nuevos_datos[i] = 1ull;
	};

	for (size_t i = (nueva_capacidad / 2) - 1; i > 0; i--) {
		size_t idx_izq = i * 2;
		size_t idx_der = i * 2 + 1;

		unsigned long long izq = (idx_izq >= ultima_hoja)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der >= ultima_hoja)? 1 : arbol->datos[idx_der];
		arbol->datos[i] = izq * der;
	};

	arbol->capacidad_datos = nueva_capacidad;
};

void merkle_realloc_nodos(Merkle* arbol, size_t nueva_capacidad) {
	if (!arbol) return;
	if (nueva_capacidad <= arbol->capacidad_nodos) return;

	Blockchain** nuevos_nodos = (Blockchain**)realloc((void*)arbol->nodos, sizeof(Blockchain*) * nueva_capacidad);
	if (!nuevos_nodos) {
		perror("no se pudo hacer realloc\n");
		exit(1);
	};

	for (size_t i = arbol->capacidad_nodos; i < nueva_capacidad; i++) {
		nuevos_nodos[i] = NULL;
	};

	arbol->nodos = nuevos_nodos;
	arbol->capacidad_nodos = nueva_capacidad;
};

size_t merkle_alta(Merkle* arbol, Blockchain *nodo) {
	if (!arbol) return 0;
	if (!nodo) return 0;

	if (arbol->cantidad_nodos >= arbol->capacidad_datos / 2) {
		merkle_realloc_datos(arbol, arbol->capacidad_datos * 2);
	};

	if (arbol->cantidad_nodos + 1 >= arbol->capacidad_nodos) {
		merkle_realloc_nodos(arbol, arbol->capacidad_nodos * 2);
	};

	if (arbol->indice_primos >= arbol->longitud_primos) {
		free(arbol->primos);
		arbol->longitud_primos *= 2;
		arbol->primos = primos((int)arbol->longitud_primos);
	};

	const size_t idx_hoja = arbol->cantidad_nodos + (arbol->capacidad_datos / 2);
	arbol->datos[idx_hoja] = arbol->primos[arbol->indice_primos++];
	arbol->nodos[arbol->cantidad_nodos] = nodo;
	arbol->cantidad_nodos++;

	size_t i = idx_hoja / 2;
	while (i > 0) {
		size_t idx_izq = i * 2;
		size_t idx_der = i * 2 + 1;

		unsigned long long izq = (idx_izq > idx_hoja)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > idx_hoja)? 1 : arbol->datos[idx_der];
		arbol->datos[i] = izq * der;
		i /= 2;
	};

	return idx_hoja - (arbol->capacidad_datos / 2);
};

void merkle_actualizar(Merkle* arbol, size_t id_blockchain, size_t id_nodo, size_t long_mensaje, char* mensaje) {
	if(arbol == NULL || id_blockchain > arbol->cantidad_nodos)
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

	size_t ultima_hoja = arbol->cantidad_nodos - 1 + (arbol->capacidad_datos / 2);
	size_t indice_mk = (arbol->capacidad_datos / 2) + id_nodo;
	arbol->datos[indice_mk] = arbol->primos[arbol->indice_primos++];
	indice_mk /= 2;

	while (indice_mk > 0) {
		size_t idx_izq = indice_mk * 2;
		size_t idx_der = indice_mk * 2 + 1;

		unsigned long long izq = (idx_izq > ultima_hoja)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > ultima_hoja)? 1 : arbol->datos[idx_der];
		arbol->datos[indice_mk] = izq * der;
		indice_mk /= 2;
	};
}

void merkle_amendar(Merkle* arbol, size_t id_blockchain, size_t id_nodo, size_t long_mensaje, char* mensaje) {
	if(arbol == NULL || id_blockchain > arbol->cantidad_nodos)
		return;

	Blockchain* indice_bl = arbol->nodos[id_blockchain];
	indice_bl = bl_agregar_final(indice_bl, id_nodo, long_mensaje, mensaje);

	if (arbol->indice_primos >= arbol->longitud_primos) {
		free(arbol->primos);
		arbol->longitud_primos *= 2;
		arbol->primos = primos((int)arbol->longitud_primos);
	};

	size_t indice_mk = (arbol->capacidad_datos / 2) + id_nodo;
	arbol->datos[indice_mk] = arbol->primos[arbol->indice_primos++];
	indice_mk /= 2;

	size_t ultima_hoja = arbol->cantidad_nodos - 1 + (arbol->capacidad_datos / 2);
	while (indice_mk > 0) {
		size_t idx_izq = indice_mk * 2;
		size_t idx_der = indice_mk * 2 + 1;

		unsigned long long izq = (idx_izq > ultima_hoja)? 1 : arbol->datos[idx_izq];
		unsigned long long der = (idx_der > ultima_hoja)? 1 : arbol->datos[idx_der];
		arbol->datos[indice_mk] = izq * der;
		indice_mk /= 2;
	};
};

unsigned long long merkle_validar_subarbol(Merkle* arbol, size_t id_nodo) {
	if (!arbol) return 0;
	if (!id_nodo) return 0;
	if (id_nodo > arbol->capacidad_datos) return 0;
	if (id_nodo >= arbol->capacidad_datos / 2) {
		if (id_nodo >= (arbol->capacidad_datos / 2) + arbol->cantidad_nodos) {
			printf("oob exit\n");
			return 1;
		} else {
			printf("inbounds exit with %llu\n", arbol->datos[id_nodo]);
			return arbol->datos[id_nodo];
		};
	};

	unsigned long long izq = merkle_validar_subarbol(arbol, id_nodo * 2);
	unsigned long long der = merkle_validar_subarbol(arbol, id_nodo * 2 + 1);
	unsigned long long valor_esperado = izq * der;
	if (valor_esperado != arbol->datos[id_nodo]) {
		printf("unexpected value between %llu, %llu (expected %llu, got %llu)\n", izq, der, valor_esperado, arbol->datos[id_nodo]);
		return 0;
	};

	printf("validated %llu\n", valor_esperado);
	return valor_esperado;
};

bool merkle_validar(Merkle* arbol) {
	if (!arbol) return false;
	for(size_t i=0; i<arbol->cantidad_nodos; i++){
		Blockchain* lista = arbol->nodos[i];
		if(lista == NULL) continue;
		if(lista->siguiente == NULL) continue;

		lista = lista->siguiente;
		while(lista != NULL) {
			if(lista->id <= lista->anterior->id)
				return false;

			lista = lista->siguiente;
		};
	};

	printf("merkle_validar_subarbol\n");
	return merkle_validar_subarbol(arbol, 1);
};

bool merkle_validar_subconjunto(Merkle* arbol, unsigned long long producto_esperado, size_t inicio, size_t fin) {
	if (!arbol) return false;
	if (inicio > fin) return false;
	if (fin >= arbol->cantidad_nodos) return false;

	size_t hojas = arbol->capacidad_datos / 2;
	unsigned long long producto = 1;

	for(size_t i=inicio; i<=fin; i++){
		producto *= arbol->datos[hojas + i];

		Blockchain* lista = arbol->nodos[i];
		if(lista == NULL) continue;
		if(lista->siguiente == NULL) continue;

		lista = lista->siguiente;
		while(lista != NULL){
			if(lista->id <= lista->anterior->id) {
				printf("failed to validate blockchain\n");
				return false;
			};

			lista = lista->siguiente;
		};
	};

	return producto == producto_esperado;
};
