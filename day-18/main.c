#include "shared.h"


typedef struct {
  Vector X;
  Vector Y;
} VectorPair;


VectorPair initVectorPair() {
  VectorPair V;
  V.X = initVector(INITIAL_CAPACITY);
  V.Y = initVector(INITIAL_CAPACITY);

  return V;
}


void printVectorPair(VectorPair *V) {
  for (int i = 0; i < V->X.length && i < V->Y.length; i++) {
    printf("%d,%d\n", V->X.x[i], V->Y.x[i]);
  }
}


void freeVectorPair(VectorPair *V) {
  freeVector(&V->X);
  freeVector(&V->Y);
}


VectorPair readfile(const char *path) {
  VectorPair V = initVectorPair();

  FILE *file = fopen(path, "r");

  ARR_TYPE num;
  int onum = 0, rhs = 0;
  char c;

  while ((c = getc(file)) != EOF) {
    if (isdigit(c)) {
      num = 10 * num + c - '0';
      onum = 1;
    } else if (onum) {
      if (rhs) append(&V.Y, num);
      else     append(&V.X, num);

      rhs = rhs ? 0: 1;
      num = 0;
    } else num = 0;
  }

  if (V.X.length != V.Y.length)
    printf("Warning: V.X and V.Y are of different lengths!\n");

  fclose(file);

  return V;
}


int validPos(int i, int j, Matrix *M) {
  return i >= 0 && i < M->nrow && j >= 0 && j < M->ncol &&
    M->x[i][j] == '.';
}


int DjikstraStep3To5(Vector *Ai, Vector *Aj, Vector *D, Matrix *M) {
  // Step 3
  int min_dist = -1, min_i = -1, min_j = -1, min_k;

  for (int k = 0; k < Ai->length; k++) {
    int i = Ai->x[k], j = Aj->x[k];

    if (M->x[i][j] != '.' || D->x[k] < 0) continue;

    if (min_dist < 0 || D->x[k] < min_dist)
      min_dist = D->x[k], min_i = i, min_j = j, min_k = k;
  }

  if (min_dist < 0) return 1;
  M->x[min_i][min_j] = 'O';

  // Step 4
  int steps[3] = {-1, 0, 1};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if ((steps[i] == 0 && steps[j] == 0) || 
          (steps[i] != 0 && steps[j] != 0)) // both cant be non-zero, or zero
        continue;

      int n_i = min_i + steps[i], 
          n_j = min_j + steps[j];

      if (!validPos(n_i, n_j, M)) 
        continue;

      int u = which2D(Ai, Aj, n_i, n_j);
      int d = D->x[u];

      if (d < 0 || d > D->x[min_k] + 1)
        D->x[u] = D->x[min_k] + 1;
    }
  }

  return 0;
}


void Djikstra(Vector *Ai, Vector *Aj, Vector *D, Matrix *M) {
  int status = 0;
  for (int i = 0; i < Ai->length && !status; i++)
    status = DjikstraStep3To5(Ai, Aj, D, M);
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file arg!\n");
    exit(-1);
  }

  int nano_sec = 12;
  int dim = 7;
  int target_i = dim - 1, target_j = dim - 1;
  VectorPair V = readfile(argv[1]);

  Matrix M = initMatrix(dim, dim);
  fillMatrix(&M, '.');

  for (int k = 0; k < nano_sec && k < V.X.length && k < V.Y.length; k++) {
    int i = V.Y.x[k], j = V.X.x[k];
    M.x[i][j] = '#';
  }
  
  Matrix N  = copyMatrix(&M);
  Vector Ai = initVector(M.nrow * M.ncol);
  Vector Aj = initVector(M.nrow * M.ncol);
  Vector D  = initVector(M.nrow * M.ncol);

  for (int i = 0; i < M.nrow; i++) {
    for (int j = 0; j < M.ncol; j++) {
      if (M.x[i][j] == '#') continue;

      append(&Ai, i);
      append(&Aj, j);

      if (i == 0 && j == 0) append(&D, 0);
      else append(&D, -1); // -1 = Inf
    }
  }

  Vector E  = copyVector(&D);
  Vector Bi = copyVector(&Ai);
  Vector Bj = copyVector(&Aj);
 
  // Task 1
  Djikstra(&Ai, &Aj, &D, &M);

  printf("Answer Task 1\n  Shortest distance = %d\n", 
      D.x[which2D(&Ai, &Aj, target_i, target_j)]);

  freeMatrix(&M);
  freeVector(&Ai);
  freeVector(&Aj);
  freeVector(&D);

  // Task 2
  for (int k = nano_sec; k < V.X.length && k < V.Y.length; k++) {
    int i = V.Y.x[k], j = V.X.x[k];
    N.x[i][j] = '#';

    printf("\rchecking byte %d [%2d,%2d]", k + 1, j, i);
    fflush(stdout);
    M  = copyMatrix(&N);
    D  = copyVector(&E);
    Ai = copyVector(&Bi);
    Aj = copyVector(&Bj);

    Djikstra(&Ai, &Aj, &D, &M);
    
    int d = D.x[which2D(&Ai, &Aj, target_i, target_j)];

    freeMatrix(&M);
    freeVector(&D);
    freeVector(&Ai);
    freeVector(&Aj);
    
    if (d == -1) {
      printf("\rAnswer Task 2\n  Breaking byte [%2d,%2d]\n", j, i);
      printMatrix(&M);
      break;
    }
  }

  freeVector(&E);
  freeVector(&Bi);
  freeVector(&Bj);
  freeMatrix(&N);
  freeVectorPair(&V);
  
  return 0;
}
