#ifndef SHARED_H

#define SHARED_H

#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"


#define INITIAL_CAPACITY 8
#define ARR_TYPE int
#define MAT_TYPE char


// array.c
typedef struct {
  ARR_TYPE *x;
  long int length;
  long int capacity;
} Vector;


typedef struct {
  long int nrow;
  long int ncol;
  MAT_TYPE **x;
} Matrix;


Vector initVector(int capacity);
void append(Vector *X, ARR_TYPE x);
void concat(Vector *X, Vector *Y);
void freeVector(Vector *X);
void printVectorNoNewline(Vector *X);
void printVector(Vector *X);
int which(Vector *X, int x);
int isin(int x, Vector *X);
void resetVector(Vector *X);
Vector copyVector(Vector *X);
int which2D(Vector *X, Vector *Y, ARR_TYPE x, ARR_TYPE y);
int isin2D(ARR_TYPE x, ARR_TYPE y, Vector *X, Vector *Y);
int isequal(Vector *X, Vector *Y);
long long firstMisMatch(Vector *X, Vector *Y);
long long lastMisMatch(Vector *X, Vector *Y);
ARR_TYPE maxVector(Vector *X);
ARR_TYPE pop(Vector *X, const int i);


// matrix.c
Matrix initMatrix(const long int nrow, const long int ncol);
void freeMatrix(Matrix *M);
void printMatrix(Matrix *M);
void fillMatrix(Matrix *M, MAT_TYPE fill);


#endif
