#ifndef  SHARED_H

#define SHARED_H
#define INIT_CAPACITY 8
#define MAX_10S 1000

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// array.c
typedef struct {
  long long int *x;
  long int capacity;
  long int size;
} Array;


Array initArray();
void resetArray(Array *X);
void append(Array *X, long long int x);
int get(Array *X, long int i);
Array initArrayN(long int capacity);
void printArray(Array *X);

#endif // ! SHARED_H
