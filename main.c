#include "stdlib.h"
#include "stdio.h"
#include "generador_primos.h"

int main() {
  // Genero y muestro los primeros 100 números primos
  int *p = primos(100);
  for(int i = 0; i < 100; i++)
    printf("%d ",p[i]);

  // Uso los números primos

  // IMPORTANTE: libero la memoria pedida para el arreglo de números primos
  free(p);
  return 0;
}
