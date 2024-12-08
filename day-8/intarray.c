#include "shared.h"


IntArray initIntArray(int capacity) {
  IntArray X;
  X.x = (int*)malloc(capacity * sizeof(int));
  X.capacity = capacity;
  X.length = 0;
  return X;
}


void appendIntArray(IntArray *X, int x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (int*)realloc(X->x, X->capacity * sizeof(int));
  }
  X->x[X->length++] = x;
}


void resetIntArray(IntArray *X) {
  X->length = 0;
}


void freeIntArray(IntArray *X) {
  free(X->x);
}


void printIntArrayNoNewline(IntArray X) {
  printf("[");
  for (int i = 0; i < X.length; i++) {
    printf("%d", X.x[i]);
    if (i + 1 < X.length) printf(", ");
  }
  printf("]");
}


void printIntArray(IntArray X) {
  printIntArrayNoNewline(X);
  printf("\n");
}
