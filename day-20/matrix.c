#include "shared.h"


// Matrix
Matrix initMatrix(const long int nrow, const long int ncol) {
  Matrix M;
  M.nrow = nrow;
  M.ncol = ncol;
  M.x = (MAT_TYPE**)malloc(M.nrow * sizeof(MAT_TYPE*));

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


void printMatrix(Matrix *M) {
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] < INT_MAX && M->x[i][j] > 0) 
        printf("%2d|", M->x[i][j]);
      else 
        printf("  |");
    }

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
