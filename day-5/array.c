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
  X.x = (int*)malloc(capacity * sizeof(int));
  X.capacity = capacity;
  X.length = 0;
  return X;
}


void AddArray(ArrayArray *X) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (Array*)realloc(X->x, X->capacity * sizeof(Array));
  }
  X->x[X->length++] = initArray(INITIAL_CAPACITY);
}


void append(Array *X, int x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (int*)realloc(X->x, X->capacity * sizeof(int));
  }
  X->x[X->length++] = x;
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


void printArrayNoNewline(Array X) {
  printf("[");
  for (int i = 0; i < X.length; i++) {
    printf("%d", X.x[i]);
    if (i + 1 < X.length) printf(", ");
  }
  printf("]");
}


void printArray(Array X) {
  printArrayNoNewline(X);
  printf("\n");
}


void printArrayArray(ArrayArray X) {
  printf("[");
  for (int i = 0; i < X.length; i++) {
    if (i) printf(" ");
    printArrayNoNewline(X.x[i]);
    if (i + 1 < X.length) printf(",\n");
  }
  printf("]\n");
}


int which(Array x, int value) {
  for (int i = 0; i < x.length; i++) if (x.x[i] == value) return(i);
  return -1;
}


int getMiddleNumber(Array x) {
  if (x.length == 0) {
    printf("Trying to get middle number in empyt array!");
    return -1;
  }
  return x.x[x.length / 2];
}
