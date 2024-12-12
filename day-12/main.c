#include "shared.h"


typedef struct {
  char c;
  Array I;
  Array J;
  int perimeter;
  int area;
  // n-corner blocks
  int n4;
  int n3;
  int n2;
} Garden;


void printGarden(Garden *G) {
  printf("Garden %c\n", G->c);
  printf("Area: %d\n", G->area);
  printf("Perimeter: %d\n", G->perimeter);
  printArray(&G->I);
  printArray(&G->J);
}


void freeGarden(Garden *G) {
  freeArray(&G->I);
  freeArray(&G->J);
}


typedef struct {
  Garden *gardens;
  int n;
} Gardens;


int ncorners(int i, int j, Map *M) {
  int next_i[4] = {i, i, i - 1, i + 1};
  int next_j[4] = {j - 1, j + 1, j, j};
  int free[4] = {0, 0, 0, 0};

  int n = 0;
  for (int k = 0; k < 4; k++) {
    int i1 = next_i[k];
    int j1 = next_j[k];
    
    if (i1 < 0 || i1 >= M->nrow || j1 < 0 || j1 >= M->ncol || M->x[i][j] != M->x[i1][j1]) {
      n++;
      free[k] = 1;
    }
  }

  // check if free = {0, 0, 1, 1} or {1, 1, 0, 0}
  if ((free[0] && free[1] && !free[2] && !free[3]) ||
      (!free[0] && !free[1] && free[2] && free[3])) n = 0;

  return n;
}


int isValidPos(int i, int j, Map *M, Garden *G) {
  return i >= 0 && i < M->nrow && j >= 0 && j < M->ncol && 
    !isin2D(i, j, &G->I, &G->J);
}


void iterGarden(int i, int j, Map *M, Garden *G) {
  int next_i[4] = {i, i, i - 1, i + 1};
  int next_j[4] = {j - 1, j + 1, j, j};

  int corners = ncorners(i, j, M);

  switch (corners) {
    case 4:
      G->n4++;
      break;
    case 3:
      G->n3++;
      break;
    case 2:
      G->n2++;
      break;
  }

  for (int k = 0; k < 4; k++) {
    int i1 = next_i[k];
    int j1 = next_j[k];
    
    if (!isValidPos(i1, j1, M, G) || M->x[i][j] != M->x[i1][j1]) {
      if (!isin2D(i1, j1, &G->I, &G->J)) G->perimeter++;
      continue;
    }
    
    G->area++;
    append(&G->I, i1);
    append(&G->J, j1);

    iterGarden(i1, j1, M, G);
  }
}


Garden mapGarden(int i, int j, Map *M) {
  Garden G;
  G.c = M->x[i][j];
  G.I = initArray(INITIAL_CAPACITY);
  G.J = initArray(INITIAL_CAPACITY);
  G.perimeter = 0;
  G.area = 1;
  G.n4 = 0;
  G.n3 = 0;
  G.n2 = 0;

  append(&G.I, i);
  append(&G.J, j);
  iterGarden(i, j, M, &G);

  return G;
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file arg!\n");
    exit(-1);
  }

  Map *M = readfile(argv[1]);
  Array I = initArray(INITIAL_CAPACITY);
  Array J = initArray(INITIAL_CAPACITY);

  printMap(M);
  long long int sum_t1 = 0, sum_t2 = 0;
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (isin2D(i, j, &I, &J)) continue;
      
      Garden G = mapGarden(i, j, M);
      concat(&I, &G.I);
      concat(&J, &G.J);

      int sides = 2 * (4 * G.n4 + 2 * G.n3 + G.n2 - 4) + 4;
      sum_t1 += G.area * G.perimeter;
      sum_t2 += G.area * sides;
      
      printf("A region of %c plants with price %d * %d = %d\n", G.c, G.area, sides, G.area * sides);
      printf("  sides = n4 = %d, n3 = %d, n2 = %d\n", G.n4, G.n3, G.n2);
      freeGarden(&G);
    }
  }
 
  printf("Answer Task 1\n  Sum = %lld\n", sum_t1);
  printf("Answer Task 2\n  Sum = %lld\n", sum_t2);
  freeMap(M);

  return 0;
}
