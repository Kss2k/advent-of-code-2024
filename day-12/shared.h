#ifndef SHARED_H

#define SHARED_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAPACITY 8
#define ARR_TYPE int
#define MAP_TYPE char


// array.c
typedef struct {
  ARR_TYPE *x;
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
void append(Array *X, ARR_TYPE x);
void concat(Array *X, Array *Y);
void freeArray(Array *X);
void freeArrayArray(ArrayArray *X);
void printArrayNoNewline(Array *X);
void printArray(Array *X);
void printArrayArray(ArrayArray X);
int which(Array *X, int x);
int isin(int x, Array *X);
void resetArray(Array *X);
Array copyArray(Array *X);
int which2D(Array *X, Array *Y, ARR_TYPE x, ARR_TYPE y);
int isin2D(ARR_TYPE x, ARR_TYPE y, Array *X, Array *Y);


// map.c
typedef struct {
  MAP_TYPE **x;
  int nrow;
  int ncol;
} Map;


Map *initMap(int nrow, int ncol);
Map *copyMap(Map *M);
void freeMap(Map *M);
void printMap(Map *M);
Map *readfile(const char *path);
int countOccurences(MAP_TYPE x, Map *M);

#endif
