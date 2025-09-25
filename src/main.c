#include <stdio.h>

#include "blockchain.h"
#include "merkle.h"

typedef struct Registro {
	char* mensaje;
	size_t longitud;
	size_t tiempo;
} Registro;

Registro registrar_deuda(size_t tiempo, char* nombre, int dni, int monto) {
	Registro deuda;
	size_t longitud = snprintf(NULL, 0, "Persona %s DNI %d debe $%d", nombre, dni, monto);
	char* mensaje = malloc(longitud);
	sprintf(mensaje, "Persona %s DNI %d debe $%d", nombre, dni, monto);
	deuda.mensaje = mensaje;
	deuda.longitud = longitud;
	deuda.tiempo = tiempo;
	return deuda;
};

Registro registrar_credito(size_t tiempo, char* nombre, int dni, int monto) {
	Registro credito;
	size_t longitud = snprintf(NULL, 0, "Persona %s DNI %d debe $%d", nombre, dni, monto);
	char* mensaje = malloc(longitud);
	sprintf(mensaje, "Persona %s DNI %d pago $%d", nombre, dni, monto);
	credito.mensaje = mensaje;
	credito.longitud = longitud;
	credito.tiempo = tiempo;
	return credito;
};

int main() {
	Merkle* arbol_deudores = merkle_crear(0, NULL);

	Registro deuda_felipe = registrar_deuda(1, "Felipe Isern", 12341245, 667500);
	Blockchain* bl_felipe = bl_agregar_inicio(NULL, deuda_felipe.tiempo, deuda_felipe.longitud, deuda_felipe.mensaje);
	size_t id_felipe = merkle_alta(arbol_deudores, bl_felipe);

	Registro deuda_mate = registrar_deuda(2, "Mateo Delmagro", 98765432, 120000);
	Blockchain* bl_mate = bl_agregar_inicio(bl_felipe, deuda_mate.tiempo, deuda_mate.longitud, deuda_mate.mensaje);
	size_t id_mate = merkle_alta(arbol_deudores, bl_mate);

	Registro deuda_manu = registrar_deuda(3, "Manuel Slepoy", 11223344, 45000);
	Blockchain* bl_manu = bl_agregar_inicio(bl_mate, deuda_manu.tiempo, deuda_manu.longitud, deuda_manu.mensaje);
	size_t id_manu = merkle_alta(arbol_deudores, bl_manu);

	if (merkle_validar(arbol_deudores)) {
		printf("Árbol Merkle válido.\n");
	} else {
		printf("Árbol Merkle inválido.\n");
	};

	Registro pago_manu = registrar_credito(4, "Manuel Slepoy", 11223344, 20000);
	merkle_amendar(arbol_deudores, id_manu, id_manu, pago_manu.longitud, pago_manu.mensaje);

	size_t producto_esperado = arbol_deudores->datos[id_felipe] * arbol_deudores->datos[id_mate];
	if (merkle_validar_subconjunto(arbol_deudores, producto_esperado, id_felipe, id_mate)) {
		printf("Subconjunto válido entre Felipe y Mate.\n");
	} else {
		printf("Subconjunto inválido.\n");
	};

	producto_esperado = arbol_deudores->datos[id_mate] * arbol_deudores->datos[id_manu];
	if (merkle_validar_subconjunto(arbol_deudores, producto_esperado, id_mate, id_manu)) {
		printf("Subconjunto válido entre Mateo y Manuel.\n");
	} else {
		printf("Subconjunto inválido.\n");
	};

	free(deuda_felipe.mensaje);
	free(deuda_mate.mensaje);
	free(deuda_manu.mensaje);
	free(pago_manu.mensaje);
	bl_destruir(bl_manu);
	merkle_destruir(arbol_deudores);
	return 0;
};
