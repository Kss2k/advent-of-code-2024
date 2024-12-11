#include "shared.h"
#include <stdio.h>


Array readfile(const char *path) {
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    printf("Could not find file!\n");
    exit(-1);
  }


  Array X = initArray(INITIAL_CAPACITY);

  long long int num = 0;
  int onnum = 0;
  char c;
  while ((c = getc(file)) != EOF) {
    if (isdigit(c)) {
      onnum = 1;
      num = 10 * num + c - '0';

    } else {
      if (onnum) append(&X, num);
      onnum = 0, num = 0;
    }
  }

  fclose(file);

  return X;
}


long long int raise(int x, int n) {
  long long int out = 1;
  for (int i = 0; i < n; i++) out *= x;
  return out;
}


int ndigits(long long int x) {
  int n = 0;
  while (pow(10.0, (double)(n)) <= x) n++;
  return n;
}


long int gdigits(long int x, int offset, int n) {
  long int num = x;
  long int out = 0;

  for (int i = 0; i < offset; i++) num /= 10;
  for (int i = 0; i < n && num; i++) {
    out += (num % 10) * pow(10.0, (double)(i));
    num /= 10; 
  }

  return out;
} 


void appendUnique(Array *X, Array *C, long long int x, long long int count) {
  if (which(X, x) == -1) {
    append(X, x);
    append(C, count);
  } else {
    C->x[which(X, x)] += count;
  }
}


typedef struct {
  Array X;
  Array C;
} State;


State getNextState(Array *X,  Array *C) {
  Array Y = initArray(X->length);
  Array K = initArray(X->length);

  int n = 0, k = 0;
  for (int i = 0; i < X->length; i++) {
    if (X->x[i] == 0) appendUnique(&Y, &K, 1, C->x[i]);
    else if ((n = ndigits(X->x[i])) % 2 == 0) {
      appendUnique(&Y, &K, gdigits(X->x[i], n / 2, n / 2), C->x[i]);
      appendUnique(&Y, &K, gdigits(X->x[i], 0, n / 2), C->x[i]);
    } else appendUnique(&Y, &K, X->x[i] * 2024, C->x[i]);
  }

  freeArray(X);
  freeArray(C);

  State S;
  S.X = Y;
  S.C = K;

  return S;
}


int main(int argc, char **argv) {
  #define ITERATIONS_T1 25
  #define ITERATIONS_T2 75

  if (argc <= 1) {
    printf("Missing file argument!\n");
    exit(-1);
  }

  Array X = readfile(argv[1]);
  Array C = copyArray(&X);
  // assuming X is unique
  for (int i = 0; i < X.length; i++) C.x[i] = 1;

  for (int i = 0; i < ITERATIONS_T1; i++) {
    State S = getNextState(&X, &C);
    X = S.X;
    C = S.C;
  }

  long long int sum = 0;
  for (int i = 0; i < X.length; i++) sum += C.x[i];
  printf("Answer Task 1:\n  Sum = %lld\n", sum);
  
  for (int i = 0; i < ITERATIONS_T2 - ITERATIONS_T1; i++) {
    State S = getNextState(&X, &C);
    X = S.X;
    C = S.C;
  }

  for (int i = 0; i < X.length; i++) sum += C.x[i];
  printf("Answer Task 2:\n  Sum = %lld\n", sum);

 
  freeArray(&X);
  freeArray(&C);

  return 0;
}
