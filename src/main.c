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
	char* mensaje = malloc(longitud + 1);
	sprintf(mensaje, "Persona %s DNI %d debe $%d", nombre, dni, monto);
	deuda.mensaje = mensaje;
	deuda.longitud = longitud;
	deuda.tiempo = tiempo;
	return deuda;
};

Registro registrar_credito(size_t tiempo, char* nombre, int dni, int monto) {
	Registro credito;
	size_t longitud = snprintf(NULL, 0, "Persona %s DNI %d debe $%d", nombre, dni, monto);
	char* mensaje = malloc(longitud + 1);
	sprintf(mensaje, "Persona %s DNI %d pago $%d", nombre, dni, monto);
	credito.mensaje = mensaje;
	credito.longitud = longitud;
	credito.tiempo = tiempo;
	return credito;
};

int main() {
	Merkle* arbol_deudores = merkle_crear(0, NULL);

	if (merkle_validar(arbol_deudores)) {
		printf("Árbol Merkle válido.\n");
	} else {
		printf("Árbol Merkle inválido.\n");
	};

	Registro deuda_felipe = registrar_deuda(1, "Felipe Isern", 48135419, 667500);
	Blockchain* bl_felipe = bl_agregar_final(NULL, deuda_felipe.tiempo, deuda_felipe.longitud, deuda_felipe.mensaje);
	size_t id_felipe = merkle_alta(arbol_deudores, bl_felipe);

	if (merkle_validar(arbol_deudores)) {
		printf("Árbol Merkle válido.\n");
	} else {
		printf("Árbol Merkle inválido.\n");
	};

	Registro deuda_mate = registrar_deuda(2, "Mateo Delmagro", 47778479, 120000);
	Blockchain* bl_mate = bl_agregar_final(NULL, deuda_mate.tiempo, deuda_mate.longitud, deuda_mate.mensaje);
	size_t id_mate = merkle_alta(arbol_deudores, bl_mate);

	if (merkle_validar(arbol_deudores)) {
		printf("Árbol Merkle válido.\n");
	} else {
		printf("Árbol Merkle inválido.\n");
	};

	Registro deuda_manu = registrar_deuda(3, "Manuel Slepoy", 48540237, 45000);
	Blockchain* bl_manu = bl_agregar_final(NULL, deuda_manu.tiempo, deuda_manu.longitud, deuda_manu.mensaje);
	size_t id_manu = merkle_alta(arbol_deudores, bl_manu);

	if (merkle_validar(arbol_deudores)) {
		printf("Árbol Merkle válido.\n");
	} else {
		printf("Árbol Merkle inválido.\n");
	};

	Registro pago_manu = registrar_credito(4, "Manuel Slepoy", 48540237, 20000);
	merkle_amendar(arbol_deudores, pago_manu.tiempo, id_manu, pago_manu.longitud, pago_manu.mensaje);

	unsigned long long producto_esperado;
	producto_esperado = arbol_deudores->datos[(arbol_deudores->capacidad_datos / 2) + id_felipe]
	                  * arbol_deudores->datos[(arbol_deudores->capacidad_datos / 2) + id_mate];
	if (merkle_validar_subconjunto(arbol_deudores, producto_esperado, id_felipe, id_mate)) {
		printf("Subconjunto válido entre Felipe y Mate.\n");
	} else {
		printf("Subconjunto inválido.\n");
	};

	producto_esperado = arbol_deudores->datos[(arbol_deudores->capacidad_datos / 2) + id_mate]
	                  * arbol_deudores->datos[(arbol_deudores->capacidad_datos / 2) + id_manu];
	if (merkle_validar_subconjunto(arbol_deudores, producto_esperado, id_mate, id_manu)) {
		printf("Subconjunto válido entre Mateo y Manuel.\n");
	} else {
		printf("Subconjunto inválido.\n");
	};

	Registro deuda_matias1 = registrar_deuda(10, "Matias Xavier Roveri", 48976714, 250000);
	Blockchain* bl_matias = bl_agregar_final(NULL, deuda_matias1.tiempo, deuda_matias1.longitud, deuda_matias1.mensaje);
	size_t id_matias = merkle_alta(arbol_deudores, bl_matias);

	Registro pago_matias1 = registrar_credito(11, "Matias Xavier Roveri", 48976714, 50000);
	merkle_amendar(arbol_deudores, pago_matias1.tiempo, id_matias, pago_matias1.longitud, pago_matias1.mensaje);

	Registro deuda_benicio1 = registrar_deuda(12, "Benicio Sánchez Mandato", 47842266, 98000);
	Blockchain* bl_benicio = bl_agregar_final(NULL, deuda_benicio1.tiempo, deuda_benicio1.longitud, deuda_benicio1.mensaje);
	size_t id_benicio = merkle_alta(arbol_deudores, bl_benicio);

	Registro deuda_benicio2 = registrar_deuda(13, "Benicio Sánchez Mandato", 47842266, 45000);
	bl_benicio = bl_agregar_final(bl_benicio, deuda_benicio2.tiempo, deuda_benicio2.longitud, deuda_benicio2.mensaje);
	merkle_amendar(arbol_deudores, deuda_benicio2.tiempo, id_benicio, deuda_benicio2.longitud, deuda_benicio2.mensaje);

	Registro deuda_luca = registrar_deuda(14, "Luca Mamani", 48136804, 1060000);
	Blockchain* bl_luca = bl_agregar_final(NULL, deuda_luca.tiempo, deuda_luca.longitud, deuda_luca.mensaje);
	size_t id_luca = merkle_alta(arbol_deudores, bl_luca);

	Registro pago_luca = registrar_credito(15, "Luca Mamani", 48136804, 15000);
	merkle_amendar(arbol_deudores, pago_luca.tiempo, id_luca, pago_luca.longitud, pago_luca.mensaje);

	Registro deuda_alejandro = registrar_deuda(16, "Alejandro Schujman", 22296290, 137000);
	Blockchain* bl_alejandro = bl_agregar_final(NULL, deuda_alejandro.tiempo, deuda_alejandro.longitud, deuda_alejandro.mensaje);
	size_t id_alejandro = merkle_alta(arbol_deudores, bl_alejandro);

	Registro deuda_delfina1 = registrar_deuda(17, "Delfina Martín", 55566777, 32000);
	Blockchain* bl_delfina = bl_agregar_final(NULL, deuda_delfina1.tiempo, deuda_delfina1.longitud, deuda_delfina1.mensaje);
	size_t id_delfina = merkle_alta(arbol_deudores, bl_delfina);

	Registro deuda_delfina2 = registrar_deuda(18, "Delfina Martín", 55566777, 44000);
	bl_delfina = bl_agregar_final(bl_delfina, deuda_delfina2.tiempo, deuda_delfina2.longitud, deuda_delfina2.mensaje);
	merkle_amendar(arbol_deudores, deuda_delfina2.tiempo, id_delfina, deuda_delfina2.longitud, deuda_delfina2.mensaje);

	Registro deuda_juan = registrar_deuda(19, "Juan Manuel Rabasedas", 66677888, 5000000);
	Blockchain* bl_juan = bl_agregar_final(NULL, deuda_juan.tiempo, deuda_juan.longitud, deuda_juan.mensaje);
	size_t id_juan = merkle_alta(arbol_deudores, bl_juan);

	Registro pago_juan = registrar_credito(20, "Juan Manuel Rabasedas", 66677888, 1000000);
	merkle_amendar(arbol_deudores, pago_juan.tiempo, id_juan, pago_juan.longitud, pago_juan.mensaje);

	Registro deuda_tomas = registrar_deuda(21, "Tomás Lujan", 46710142, 27000);
	Blockchain* bl_tomas = bl_agregar_final(NULL, deuda_tomas.tiempo, deuda_tomas.longitud, deuda_tomas.mensaje);
	size_t id_tomas = merkle_alta(arbol_deudores, bl_tomas);

	Registro pago_tomas = registrar_credito(22, "Tomás Lujan", 46710142, 27000);
	merkle_amendar(arbol_deudores, pago_tomas.tiempo, id_tomas, pago_tomas.longitud, pago_tomas.mensaje);

	Registro pago_benicio_extra = registrar_credito(23, "Benicio Sánchez Mandato", 47842266, 20000);
	merkle_amendar(arbol_deudores, pago_benicio_extra.tiempo, id_benicio, pago_benicio_extra.longitud, pago_benicio_extra.mensaje);

	Registro pago_delfina_extra = registrar_credito(24, "Delfina Martín", 55566777, 12000);
	merkle_amendar(arbol_deudores, pago_delfina_extra.tiempo, id_delfina, pago_delfina_extra.longitud, pago_delfina_extra.mensaje);

	if (merkle_validar(arbol_deudores)) {
		printf("Árbol Merkle válido tras agregar más registros.\n");
	} else {
		printf("Árbol Merkle inválido tras agregar más registros.\n");
	};

	merkle_destruir(arbol_deudores);
	free(deuda_alejandro.mensaje);
	free(deuda_benicio1.mensaje);
	free(deuda_benicio2.mensaje);
	free(deuda_delfina1.mensaje);
	free(deuda_delfina2.mensaje);
	free(deuda_felipe.mensaje);
	free(deuda_juan.mensaje);
	free(deuda_luca.mensaje);
	free(deuda_manu.mensaje);
	free(deuda_mate.mensaje);
	free(deuda_matias1.mensaje);
	free(deuda_tomas.mensaje);
	free(pago_benicio_extra.mensaje);
	free(pago_delfina_extra.mensaje);
	free(pago_juan.mensaje);
	free(pago_luca.mensaje);
	free(pago_manu.mensaje);
	free(pago_matias1.mensaje);
	free(pago_tomas.mensaje);
	return 0;
};
