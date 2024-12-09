#ifndef SHARED_H

#define SHARED_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAPACITY 8
#define TYPE int


// array.c
typedef struct {
  TYPE *x;
  long int length;
  long int capacity;
} Array;


typedef struct {
  Array *x;
  long int length;
  long int capacity;
} ArrayArray;


typedef struct {
  Array before;
  Array after;
  ArrayArray orders;
} Arrays;


ArrayArray initArrayArray(int capacity);
Array initArray(int capacity);
void AddArray(ArrayArray *X);
void append(Array *X, TYPE x);
void freeArray(Array *X);
void freeArrayArray(ArrayArray *X);
void printArrayNoNewline(Array *X);
void printArray(Array *X);
void printArrayArray(ArrayArray X);
int which(Array *X, int x);
int isin(int x, Array *X);
void resetArray(Array *X);
Array copyArray(Array *X);


#endif
