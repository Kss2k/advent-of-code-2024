#ifndef SHARED_H

#define SHARED_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAPACITY 8


typedef struct {
  int *x;
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


// array.c
ArrayArray initArrayArray(int capacity);
Array initArray(int capacity);
void AddArray(ArrayArray *X);
void append(Array *X, int x);
void freeArray(Array *X);
void freeArrayArray(ArrayArray *X);
void printArrayNoNewline(Array X);
void printArray(Array X);
void printArrayArray(ArrayArray X);
int which(Array x, int value);
int isin(int x, int y, Array X, Array Y);

#endif
