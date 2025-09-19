#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generador_primos.h"

// Programa que utiliza el algoritmo 'Criba de Eratóstenes' para generar los primeros n números primos
int* primos(int n) {
    if (n <= 0) return NULL;

    // Cota superior para el nésimo primo según el teorema de Rosser: n*(log n + log log n)
    int cota_sup = (n < 6) ? 15 : (int)(n * (log(n) + log(log(n)))) + 10;

    char *es_primo = malloc(sizeof(char) * (cota_sup + 1));
    for (int i = 2; i <= cota_sup; i++) es_primo[i] = 1;

    for (int p = 2; p * p <= cota_sup; p++) {
        if (es_primo[p]) {
            for (int j = p * p; j <= cota_sup; j += p) {
                es_primo[j] = 0;
            }
        }
    }

    int *primo = malloc(sizeof(int) * n);
    int cant_primos = 0;
    for (int i = 2; i <= cota_sup && cant_primos < n; i++) {
        if (es_primo[i]) {
            primo[cant_primos++] = i;
        }
    }

    free(es_primo);
    return primo;
}
