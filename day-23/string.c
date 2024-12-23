#include "shared.h"


String initString(int capacity) {
  String S;
  S.x = (char *)malloc(capacity * sizeof(char));
  S.capacity = capacity;
  S.length = 0;
  return S;
}


void addChar(String *S, char c) {
  if (S->length == S->capacity) {
    S->capacity *= 2;
    S->x = (char *)realloc(S->x, S->capacity * sizeof(char));
  }
  S->x[S->length++] = c;
}


void paste(String *S, String *T) {
  for (int i = 0; i < T->length; i++) addChar(S, T->x[i]);
}


String substr(const String *S, int i, int n) {
  int length = n - i;
  String Y;
  Y.capacity = length;
  Y.length = length;
  Y.x = S->x+i;
  return Y;
}


void freeString(String *S) {
  free(S->x);
  S->length = 0;
  S->capacity = 0;
}


String copyString(String *S) {
  String Y;
  Y.x = (char*)malloc(S->capacity * sizeof(char));
  Y.length = S->length;
  Y.capacity = S->capacity;
  memcpy(Y.x, S->x, S->length * sizeof(char));
  return Y;
}


void printString(String *S) {
  for (int i = 0; i < S->length; i++) printf("%c", S->x[i]);
}


int equalStrings(String *X, String *Y) {
  if (X->length != Y->length) return 0;
  for (int i = 0; i < X->length; i++) if (X->x[i] != Y->x[i]) return 0;

  return 1;
}


StringVector initStringVector(int capacity) {
  StringVector X;
  X.x = (String *)malloc(capacity * sizeof(String));
  X.capacity = capacity;
  X.length = 0;
  return X;
}


StringVector copyStringVector(StringVector *X) {
  StringVector Y;
  Y.x = (String *)malloc(X->capacity * sizeof(String));
  Y.length = X->length;
  Y.capacity = X->capacity;
  // memcpy(Y.x, X->x, X->length * sizeof(String));
  for (int i = 0; i < X->length; i++)
    Y.x[i] = copyString(&X->x[i]);
  return Y;
}


void appendString(StringVector *X, String *x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (String *)realloc(X->x, X->capacity * sizeof(String));
  }
  X->x[X->length++] = *x;
}


void concatStringVectors(StringVector *X, StringVector *Y) {
  for (int i = 0; i < Y->length; i++) {
    appendString(X, &Y->x[i]);
  }
}


void resetStringVector(StringVector *X) {
  X->length = 0;
}


void freeStringVector(StringVector *X) { 
  for (int i = 0; i < X->length; i++) freeString(&X->x[i]);
  free(X->x); 
}


void printStringVectorNoNewline(StringVector *X) {
  printf("[");
  for (int i = 0; i < X->length; i++) {
    printString(&X->x[i]);
    if (i + 1 < X->length)
      printf(", ");
  }
  printf("]");
}


void printStringVector(StringVector *X) {
  printStringVectorNoNewline(X);
  printf("\n");
}


int whichStringVector(StringVector *X, String x) {
  for (int i = 0; i < X->length; i++)
    if (equalStrings(&X->x[i], &x))
      return (i);
  return -1;
}


int isinStringVector(String x, StringVector *X) { 
  return whichStringVector(X, x) != -1; 
}


int which2DStringVectors(StringVector *X, StringVector *Y, String x, String y) {
  if (X->length != Y->length) {
    printf("which2D() called on arrays of unequal lengths!\n");
  }

  for (int i = 0; i < X->length; i++) {
    if (equalStrings(&x, &X->x[i]) && 
        equalStrings(&y, &Y->x[i]))
      return i;
  }

  return -1;
}


int isin2DStringVectors(String x, String y, StringVector *X, StringVector *Y) {
  return which2DStringVectors(X, Y, x, y) != -1;
}


int isequalStringVectors(StringVector *X, StringVector *Y) {
  if (X->length != Y->length) return 0;
  for (int i = 0; i < X->length; i++) 
    if (!equalStrings(&X->x[i], &Y->x[i])) return 0;

  return 1;
}


String popString(StringVector *X, const int i) {
  if (i < 0 || i >= X->length) {
    printf("Warning: Trying to pop value outside of range (%d)!\n", i);
    printf("StringVector:\n");
    printStringVector(X);
    return initString(0);
  }

  String x = X->x[i];
  for (int j = i; j < X->length - 1; j++) {
    X->x[j] = X->x[j + 1]; // in the case where i is the last element, 
                           // we do nothing, and decrement only X->length
  }

  X->length--;

  return x;
}
