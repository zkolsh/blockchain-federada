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

	merkle_destruir(arbol_deudores);
	return 0;
};
