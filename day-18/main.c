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
    M->x[i][j] != '#';
}


int DjikstraStep3To5(Vector *Ui, Vector *Uj, Vector *Ai, Vector *Aj, Vector *D, Matrix *M) {
  // Step 3
  int min_dist = -1, min_i = -1, min_j = -1;
  
  for (int k = 0; k < Ui->length; k++) {
    int i = Ui->x[k], j = Uj->x[k];
    int u = which2D(Ai, Aj, i, j);

    if (D->x[u] < 0) continue;

    if (min_dist < 0 || (D->x[u] >= 0 && D->x[u] < min_dist)) {
      min_dist = D->x[u], min_i = i, min_j = j;
    }
  }

  if (min_dist < 0) return 1;

  // Step 4
  int steps[3] = {-1, 0, 1};
  Vector Ni = initVector(4);
  Vector Nj = initVector(4);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if ((steps[i] == 0 && steps[j] == 0) || 
          (steps[i] != 0 && steps[j] != 0)) // both cant be non-zero, or zero
        continue;

      int n_i = min_i + steps[i], 
          n_j = min_j + steps[j];

      if (!validPos(n_i, n_j, M) || which2D(Ui, Uj, n_i, n_j) == -1) 
        continue;

      append(&Ni, n_i);
      append(&Nj, n_j);
    }
  }


  for (int k = 0; k < Ni.length; k++) {
    int i = Ni.x[k], j = Nj.x[k];

    int u = which2D(Ai, Aj, i, j);
    int c = which2D(Ai, Aj, min_i, min_j);
    int d = D->x[u];

    if (d < 0 || d > D->x[c] + 1)
      D->x[u] = D->x[c] + 1;
  }

  // Step 5
  int p = which2D(Ui, Uj, min_i, min_j);
  pop(Ui, p);
  pop(Uj, p);

  freeVector(&Ni);
  freeVector(&Nj);

  return 0;
}


void Djikstra(Vector *Ui, Vector *Uj, Vector *Ai, Vector *Aj, Vector *D, Matrix *M) {
  int status = 0;
  while (Ui->length > 0 && Uj->length > 0 && !status) 
    status = DjikstraStep3To5(Ui, Uj, Ai, Aj, D, M);
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file arg!\n");
    exit(-1);
  }

  #define DIM 71
  #define NANO_SECONDS 1024


  VectorPair V = readfile(argv[1]);

  Matrix M = initMatrix(DIM, DIM);
  fillMatrix(&M, '.');

  for (int k = 0; k < NANO_SECONDS && k < V.X.length && k < V.Y.length; k++) {
    int i = V.Y.x[k], j = V.X.x[k];

    if (i < 0 || i >= M.nrow || j < 0 || j >= M.ncol) {
      printf("Attempting to fill value outside of Matrix!\n");
      continue;
    }

    M.x[i][j] = '#';
  }

  Vector Ui = initVector(M.nrow * M.ncol);
  Vector Uj = initVector(M.nrow * M.ncol);
  Vector Ai = initVector(M.nrow * M.ncol);
  Vector Aj = initVector(M.nrow * M.ncol);
  Vector D  = initVector(M.nrow * M.ncol);

  int target_i = 70, target_j = 70;
  for (int i = 0; i < M.nrow; i++) {
    for (int j = 0; j < M.ncol; j++) {
      append(&Ui, i);
      append(&Uj, j);
      append(&Ai, i);
      append(&Aj, j);

      if (i == 0 && j == 0) append(&D, 0);
      else append(&D, -1); // -1 = Inf
    }
  }

  Djikstra(&Ui, &Uj, &Ai, &Aj, &D, &M);

  printf("Answer Task 1\n  Shortest distance = %d\n", 
      D.x[which2D(&Ai, &Aj, target_i, target_j)]);

  freeVector(&Ui);
  freeVector(&Uj);
  freeVector(&Ai);
  freeVector(&Aj);

  // printMatrix(&M);
  freeMatrix(&M);
  freeVectorPair(&V);
  
  return 0;
}
