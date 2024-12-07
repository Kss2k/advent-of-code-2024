#include "shared.h"
#include <stdio.h>


Array initArray() {
  Array X;
  X.size = 0;
  X.capacity = INIT_CAPACITY;
  X.x = (long long int*)malloc(INIT_CAPACITY * sizeof(long long int));

  return X;
}


Array initArrayN(long int capacity) {
  Array X;
  X.size = 0;
  X.capacity = capacity;
  X.x = (long long int*)malloc(capacity * sizeof(long long int));

  return X;
}


void resetArray(Array *X) {
  X->size = 0;
}


void append(Array *X, long long int x) {
  if (X->size == X->capacity) {
    X->x = realloc(X->x, 2 * X->capacity * sizeof(long long int));
  }

  X->x[X->size++] = x;
}


int get(Array *X, long int i) {
  if (i >= X->size) {
    printf("Tried to access elements out of bounds!\n");
    exit(-1);
  }

  return X->x[i];
}


void printArray(Array *X) {
  printf("[");
  for (int i = 0; i < X->size; i++) {
    printf("%lld", X->x[i]);
    if (i + 1 != X->size) printf(", ");
  }
  printf("]\n");
}
