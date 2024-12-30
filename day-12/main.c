#include "shared.h"
#include <stdio.h>


#define RESET_GARDEN(G) \
  do { \
    G.sides     = 0; \
    G.area      = 0; \
    G.perimeter = 0; \
  } while (0) 


int steps_i[4] = {0, 0, -1, 1},
    steps_j[4] = {-1, 1, 0, 0};


typedef struct {
  int area;
  int perimeter;
  int sides;
} Garden;


void printMap(Matrix *M) {
  printf("Map(%ld, %ld):\n", M->nrow, M->ncol);
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      printf("%c", M->x[i][j]);
    }
    printf("\n");
  }
}


int inbounds(int i0, int j0, Matrix *M) {
  return i0 >= 0 && i0 < M->nrow && j0 >= 0 && j0 < M->ncol;
}


int vsides(int i0, int j0, Matrix *M) {
  int left = !inbounds(i0, j0 - 1, M) || M->x[i0][j0 - 1] != M->x[i0][j0],
    right = !inbounds(i0, j0 + 1, M) || M->x[i0][j0 + 1] != M->x[i0][j0];
  return left + right;
}


int hsides(int i0, int j0, Matrix *M) {
  int up = !inbounds(i0 - 1, j0, M) || M->x[i0 - 1][j0] != M->x[i0][j0],
    down = !inbounds(i0 + 1, j0, M) || M->x[i0 + 1][j0] != M->x[i0][j0];
  return up + down;
}


int nsides(int i0, int j0, Matrix *M) {
  return hsides(i0, j0, M) + vsides(i0, j0, M);
}


int nFreeCorners(int i0, int j0, Matrix *M) {
  int lu = !inbounds(i0 - 1, j0 - 1, M) || M->x[i0 - 1][j0 - 1] != M->x[i0][j0],
    ld = !inbounds(i0 + 1, j0 - 1, M) || M->x[i0 + 1][j0 - 1] != M->x[i0][j0],
    ru = !inbounds(i0 - 1, j0 + 1, M) || M->x[i0 - 1][j0 + 1] != M->x[i0][j0],
    rd = !inbounds(i0 + 1, j0 + 1, M) || M->x[i0 + 1][j0 + 1] != M->x[i0][j0];

  return lu + ld + ru + rd;
}


int isOuterCorner(int i0, int j0, Matrix *M) {
  return nsides(i0, j0, M) == 2;
}


int isInnerCorner(int i0, int j0, Matrix *M) {
  return nsides(i0, j0, M) == 0 && nFreeCorners(i0, j0, M) == 1;
}


int iscorner(int i0, int j0, Matrix *M) {
  return isOuterCorner(i0, j0, M) || isInnerCorner(i0, j0, M);
}


Matrix readfile(char *path) {
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    printf("Error: Unable to open file!\n");
    exit(-1);  
  }

  int ncol = 0, nrow = 0;

  char c;
  while ((c = getc(file)) != EOF) {
    if (c != '\n' && !nrow) ncol++;
    else if (c == '\n') nrow++;
  }

  Matrix M = initMatrix(2 * nrow, 2 * ncol);
 
  fseek(file, 0L, SEEK_SET);
  
  int i = 0, j = 0;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      i++, j = 0;
      continue;
    }

    int i0 = 2 * i, j0 = 2 * j++, 
        i1 = i0 + 1, j1 = j0 + 1;

    M.x[i0][j0] = M.x[i0][j1] = M.x[i1][j0] = M.x[i1][j1] = c;
  }

  fclose(file);

  return M;
}


void iterate(Matrix *M, Matrix *V, Garden *G, int i0, int j0, char c) {
  if (!inbounds(i0, j0, M) || M->x[i0][j0] != c || V->x[i0][j0] == '.') return;
  V->x[i0][j0] = '.';

  G->perimeter += nsides(i0, j0, M);
  G->sides += iscorner(i0, j0, M);
  G->area++;

  for (int k = 0; k < 4; k++) {
    int i1 = i0 + steps_i[k], j1 = j0 + steps_j[k];
    iterate(M, V, G, i1, j1, c);
  }
}




int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    exit(-1);
  }

  Matrix M = readfile(argv[1]);
  Matrix V = copyMatrix(&M);
  printMap(&M);

  Garden G;
 
  long long int sum_t1 = 0, perim = 0, area = 0, ngardens = 0, sum_t2 = 0, sides = 0;
  for (int i = 0; i < M.nrow; i++) for (int j = 0; j < M.ncol; j++) {
    if (V.x[i][j] == '.') continue;
    RESET_GARDEN(G); 
    ngardens++;
    iterate(&M, &V, &G, i, j, M.x[i][j]);
    printf("A region of %c plants with price %d * %d = %d\n", M.x[i][j],
         G.area,  G.perimeter, (G.perimeter / 2) * (G.area / 4));

    perim  += G.perimeter / 2;
    area   += G.area / 4;
    sides  += G.sides;
    sum_t1 += (G.perimeter / 2) * (G.area / 4);
    sum_t2 += (G.area / 4) * G.sides;
    if (G.sides < 0) printf("%d\n", G.sides);
  }

  printf("Answer Task 1:\n  Sum = %lld, perim = %lld, area = %lld, ngardens = %lld\n", sum_t1, perim, area, ngardens);
  printf("Answer Task 2:\n  Sum = %lld, sides = %lld\n", sum_t2, sides);
  freeMatrix(&M);
  freeMatrix(&V);
  return 0;
}
