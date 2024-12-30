#include "shared.h"
#include "limits.h"

Vector initVector(int capacity) {
  Vector X;
  X.x = (ARR_TYPE *)malloc(capacity * sizeof(ARR_TYPE));
  X.capacity = capacity;
  X.length = 0;
  return X;
}

Vector copyVector(Vector *X) {
  Vector Y;
  Y.x = (ARR_TYPE *)malloc(X->capacity * sizeof(ARR_TYPE));
  Y.length = X->length;
  Y.capacity = X->capacity;
  memcpy(Y.x, X->x, X->length * sizeof(ARR_TYPE));
  return Y;
}


void append(Vector *X, ARR_TYPE x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (ARR_TYPE *)realloc(X->x, X->capacity * sizeof(ARR_TYPE));
  }
  X->x[X->length++] = x;
}


void concat(Vector *X, Vector *Y) {
  for (int i = 0; i < Y->length; i++) {
    append(X, Y->x[i]);
  }
}


void resetVector(Vector *X) {
  X->length = 0;
}

void freeVector(Vector *X) { 
  free(X->x); 
}


void printVectorNoNewline(Vector *X) {
  printf("[");
  for (int i = 0; i < X->length; i++) {
    printf("%ld", X->x[i]);
    if (i + 1 < X->length)
      printf(", ");
  }
  printf("]");
}


void printVector(Vector *X) {
  printVectorNoNewline(X);
  printf("\n");
}
