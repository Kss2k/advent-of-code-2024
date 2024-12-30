#ifndef SHARED_H

#define SHARED_H

#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"


typedef struct {
  long int x[2];
} TwoDVector;


typedef struct {
  TwoDVector position;
  TwoDVector velocity;
} Guard;


#define INITIAL_CAPACITY 8
#define ARR_TYPE Guard
#define MAT_TYPE int


// string.c
typedef struct {
  char *x;
  long int length;
  long int capacity;
} String;


typedef struct {
  String *x;
  long int length;
  long int capacity;
} StringVector;


String initString(int capacity);
void addChar(String *S, char c);
void paste(String *S, String *T);
void freeString(String *S);
void printString(String *S);
String copyString(String *S);
String substr(const String *S, int i, int n);
int equalStrings(String *X, String *Y);
StringVector initStringVector(int capacity);
StringVector copyStringVector(StringVector *X);
void appendString(StringVector *X, String *x);
void concatStringVectors(StringVector *X, StringVector *Y);
void resetStringVector(StringVector *X);
void freeStringVector(StringVector *X);
void printStringVectorNoNewline(StringVector *X);
void printStringVector(StringVector *X);
int whichStringVector(StringVector *X, String x);
int isinStringVector(String x, StringVector *X);
int which2DStringVectors(StringVector *X, StringVector *Y, String x, String y);
int isin2DStringVectors(String x, String y, StringVector *X, StringVector *Y);
int isequalStringVectors(StringVector *X, StringVector *Y);
String popString(StringVector *X, const int i);

// array.c
typedef struct {
  ARR_TYPE *x;
  long int length;
  long int capacity;
} Vector;

Vector initVector(int capacity);
void append(Vector *X, ARR_TYPE x);
void concat(Vector *X, Vector *Y);
void freeVector(Vector *X);
void printVectorNoNewline(Vector *X);
void printVector(Vector *X);
int which(Vector *X, ARR_TYPE x);
int isin(ARR_TYPE x, Vector *X);
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
typedef struct {
  long int nrow;
  long int ncol;
  long int rowCapacity;
  MAT_TYPE **x;
} Matrix;

Matrix initMatrix(const long int nrow, const long int ncol);
Matrix copyMatrix(Matrix *M);
void freeMatrix(Matrix *M);
void printMatrix(Matrix *M);
void fillMatrix(Matrix *M, MAT_TYPE fill);
void addRow(Matrix *M);

#endif
