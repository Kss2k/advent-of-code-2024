#include "shared.h"


typedef struct {
  Matrix keys;
  Matrix locks;
} KeysAndLocks;


void parseBlock(FILE *file, Matrix *M, char pattern, int n, int increment) {
  addRow(M);
 
  for (int i = 0; i < M->ncol; i++) 
    M->x[M->nrow - 1][i] = -1;

  // skip first row
  char c;
  while ((c = getc(file)) != '\n') {
    if (c != EOF) continue;
    printf("Unexpected EOF!\n");
    return;
  }


  int newlines = 0, i = 0, k = M->nrow - 1;
  while ((c = getc(file)) != EOF && newlines < 2) {
    if (c == '\n') newlines++, n += increment, i = 0;
    else {
      newlines = 0;
      if (M->x[k][i++] == -1 && c != pattern) 
        M->x[k][i - 1] = n;
    }
  }
}


void addKey(FILE *file, Matrix *keys) {
  parseBlock(file, keys, '.', 5, -1);
}


void addLock(FILE *file, Matrix *locks) {
  parseBlock(file, locks, '#', 0, 1);
}


KeysAndLocks readfile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  char c; 
  long int ncol;
  while ((c = getc(file)) != '\n') 
    ncol++;  

  Matrix keys = initMatrix(0L, ncol), locks = initMatrix(0L, ncol);

  fseek(file, 0, SEEK_SET);
  
  while ((c = getc(file)) != EOF) {
    switch (c) {
      case '.':
        addKey(file, &keys);
        break;
      case '#':
        addLock(file, &locks);
        break;
    } 

    // printf("Keys:\n");
    // printMatrix(&keys);
    // printf("Locks:\n");
    // printMatrix(&locks);
  }

  fclose(file);
  KeysAndLocks K;
  K.keys = keys, K.locks = locks;
  return K;
}



int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    return -1;
  } 
  
  KeysAndLocks K = readfile(argv[1]);

  printf("Keys:\n");
  printMatrix(&K.keys);
  printf("Locks:\n");
  printMatrix(&K.locks);
  int sum_t1 = 0;

  for (int k = 0; k < K.keys.nrow; k++) for (int l = 0; l < K.locks.nrow; l++) {
    int overlap = 0;
    for (int i = 0; i < K.keys.ncol; i++)
      if (K.locks.x[l][i] + K.keys.x[k][i] > 5)
        overlap = 1;
   
    if (!overlap) sum_t1++;
  }

  printf("Answer Task 1:\n  Sum = %d\n", sum_t1);
  freeMatrix(&K.keys), freeMatrix(&K.locks);

  return 0;
}
