#ifndef SHARED_H

#define SHARED_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAPACITY 8
#define TYPE char


// array.c
typedef struct {
  int *x;
  long int length;
  long int capacity;
} IntArray;


IntArray initIntArray(int capacity);
void appendIntArray(IntArray *X, int x);
void freeIntArray(IntArray *X);
void printIntArrayNoNewline(IntArray X);
void printIntArray(IntArray X);
void resetIntArray(IntArray *X);


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
void printArrayNoNewline(Array X);
void printArray(Array X);
void printArrayArray(ArrayArray X);
int which(Array x, int value);
int isin(int x, Array X);
void resetArray(Array *X);


// map.c
typedef struct {
  char **x;
  int nrow;
  int ncol;
} Map;


Map *initMap(int nrow, int ncol);
Map *copyMap(Map *M);
void freeMap(Map *M);
void printMap(Map *M);
Map *readfile(const char *path);
void getLocations(char x, Map *M, IntArray *I, IntArray *J);
int countOccurences(char x, Map *M);


#endif
