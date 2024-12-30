#include "shared.h"


// Matrix
Matrix initMatrix(const long int nrow, const long int ncol) {
  Matrix M;
  M.rowCapacity = nrow ? nrow : 1;
  M.nrow = nrow;
  M.ncol = ncol;
  M.x = (MAT_TYPE**)malloc(M.rowCapacity * sizeof(MAT_TYPE*));

  for (int i = 0; i < nrow; i++) {
    M.x[i] = (MAT_TYPE*)malloc(M.ncol * sizeof(MAT_TYPE));
  }

  return M;
}


Matrix copyMatrix(Matrix *M) {
  Matrix N = initMatrix(M->nrow, M->ncol);
  for (int i = 0; i < M->nrow; i++) {
    memcpy(N.x[i], M->x[i], M->ncol * sizeof(MAT_TYPE));
  }

  return N;
}


void freeMatrix(Matrix *M) {
  for (int i = 0; i < M->nrow; i++) {
    free(M->x[i]);
  }
  free(M->x);
}


void addRow(Matrix *M) {
  if (M->nrow >= M->rowCapacity) {
    long int newCapacity = 2 * M->rowCapacity;
    M->rowCapacity = newCapacity;
    M->x = (MAT_TYPE**)realloc(M->x, sizeof(MAT_TYPE*) * newCapacity);
  }
 
  M->x[M->nrow] = (MAT_TYPE*)realloc(M->x[M->nrow], sizeof(MAT_TYPE) * M->ncol);
  M->nrow++;
}




void appendRow(Matrix *M, Vector *X) {
  if (X->length != M->ncol) {
    printf("Warning: Vector length does not match matrix column count!\n");
    return;
  }

  addRow(M);
  long int k = M->nrow - 1;
  for (int i = 0; i < X->length; i++)
    M->x[k][i] = X->x[i];
}


void printMatrix(Matrix *M) {
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++)
      printf("%2d|", M->x[i][j]);

    printf("\n");
    for (int k = 0; k < M->ncol; k++) printf("---");
    printf("\n");
  }
}


void fillMatrix(Matrix *M, MAT_TYPE fill) {
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      M->x[i][j] = fill;
    }
  }
}
