#include "shared.h"


ArrayArray initArrayArray(int capacity) {
  ArrayArray X;
  X.capacity = capacity;
  X.x = (Array*)malloc(capacity * sizeof(Array));
  X.length = 0;
  return X;
}


Array initArray(int capacity) {
  Array X;
  X.x = (TYPE*)malloc(capacity * sizeof(TYPE));
  X.capacity = capacity;
  X.length = 0;
  return X;
}


Array copyArray(Array *X) {
  Array Y;
  Y.x = (TYPE*)malloc(X->length * sizeof(TYPE));
  Y.length = X->length;
  Y.capacity = X->capacity;
  memcpy(Y.x, X->x, X->length * sizeof(TYPE));
  return Y;
}


void AddArray(ArrayArray *X) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (Array*)realloc(X->x, X->capacity * sizeof(Array));
  }
  X->x[X->length++] = initArray(INITIAL_CAPACITY);
}


void append(Array *X, TYPE x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (TYPE*)realloc(X->x, X->capacity * sizeof(TYPE));
  }
  X->x[X->length++] = x;
}


void resetArray(Array *X) {
  X->length = 0;
}


void freeArray(Array *X) {
  free(X->x);
}


void freeArrayArray(ArrayArray *X) {
  for (int i = 0; i < X->length; i++) {
    freeArray(&X->x[i]);
  }
  free(X->x);
}


void printArrayNoNewline(Array *X) {
  printf("[");
  for (int i = 0; i < X->length; i++) {
    printf("%c", X->x[i]);
    if (i + 1 < X->length) printf(", ");
  }
  printf("]");
}


void printArray(Array *X) {
  printArrayNoNewline(X);
  printf("\n");
}


void printArrayArray(ArrayArray X) {
  printf("[");
  for (int i = 0; i < X.length; i++) {
    if (i) printf(" ");
    printArrayNoNewline(&X.x[i]);
    if (i + 1 < X.length) printf(",\n");
  }
  printf("]\n");
}


int which(Array *X, int x) {
  for (int i = 0; i < X->length; i++) if (X->x[i] == x) return(i);
  return -1;
}


int isin(int x, Array *X) {
  return which(X, x) != -1;
}
