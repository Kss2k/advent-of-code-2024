#include "shared.h"

ArrayArray initArrayArray(int capacity) {
  ArrayArray X;
  X.capacity = capacity;
  X.x = (Array *)malloc(capacity * sizeof(Array));
  X.length = 0;
  return X;
}

Array initArray(int capacity) {
  Array X;
  X.x = (ARR_TYPE *)malloc(capacity * sizeof(ARR_TYPE));
  X.capacity = capacity;
  X.length = 0;
  return X;
}

Array copyArray(Array *X) {
  Array Y;
  Y.x = (ARR_TYPE *)malloc(X->capacity * sizeof(ARR_TYPE));
  Y.length = X->length;
  Y.capacity = X->capacity;
  memcpy(Y.x, X->x, X->length * sizeof(ARR_TYPE));
  return Y;
}

void AddArray(ArrayArray *X) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (Array *)realloc(X->x, X->capacity * sizeof(Array));
  }
  X->x[X->length++] = initArray(INITIAL_CAPACITY);
}

void append(Array *X, ARR_TYPE x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (ARR_TYPE *)realloc(X->x, X->capacity * sizeof(ARR_TYPE));
  }
  X->x[X->length++] = x;
}


void concat(Array *X, Array *Y) {
  for (int i = 0; i < Y->length; i++) {
    append(X, Y->x[i]);
  }
}


void resetArray(Array *X) { X->length = 0; }

void freeArray(Array *X) { free(X->x); }

void freeArrayArray(ArrayArray *X) {
  for (int i = 0; i < X->length; i++) {
    freeArray(&X->x[i]);
  }
  free(X->x);
}

void printArrayNoNewline(Array *X) {
  printf("[");
  for (int i = 0; i < X->length; i++) {
    printf("%d", X->x[i]);
    if (i + 1 < X->length)
      printf(", ");
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
    if (i)
      printf(" ");
    printArrayNoNewline(&X.x[i]);
    if (i + 1 < X.length)
      printf(",\n");
  }
  printf("]\n");
}

int which(Array *X, ARR_TYPE x) {
  for (int i = 0; i < X->length; i++)
    if (X->x[i] == x)
      return (i);
  return -1;
}

int isin(ARR_TYPE x, Array *X) { return which(X, x) != -1; }


int which2D(Array *X, Array *Y, ARR_TYPE x, ARR_TYPE y) {
  if (X->length != Y->length) {
    printf("which2D() called on arrays of unequal lengths!\n");
  }

  for (int i = 0; i < X->length; i++) {
    if (x == X->x[i] && y == Y->x[i])
      return i;
  }

  return -1;
}


int isin2D(ARR_TYPE x, ARR_TYPE y, Array *X, Array *Y) {
  return which2D(X, Y, x, y) != -1;
}


long long firstMisMatch(Array *X, Array *Y) {
  if (X->length != Y->length) return -2;
  for (int i = 0; i < X->length; i++) if (X->x[i] != Y->x[i]) return i;
  return -1;
}


long long lastMisMatch(Array *X, Array *Y) {
  if (X->length != Y->length) return -2;
  for (int i = X->length - 1; i >= 0; i--) if (X->x[i] != Y->x[i]) return i;
  return -1;
}


int isequal(Array *X, Array *Y) {
  if (X->length != Y->length) return 0;
  for (int i = 0; i < X->length; i++) if (X->x[i] != Y->x[i]) return 0;

  return 1;
}
