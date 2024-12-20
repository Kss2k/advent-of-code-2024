#include "shared.h"
#include <stdio.h>


#define WALL_SQUARE  -1
#define EMPTY_SQUARE -2
#define START_SQUARE -3
#define END_SQUARE   -4
#define UNVISITED    -5
#define VISITED      -6


void printMap(Matrix *M) {
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] < 0) {
        switch (M->x[i][j]) {
          case WALL_SQUARE:
            printf("#");
            break;
          case EMPTY_SQUARE:
            printf(".");
            break;
          case START_SQUARE:
            printf("S");
            break;
          case END_SQUARE:
            printf("E");
            break;
        }
      } else printf("%d", M->x[i][j]);
    }

    printf("\n");
  }
}


Matrix getVisitedMatrix(Matrix *M) {
  Matrix V = initMatrix(M->nrow, M->ncol);

  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] != WALL_SQUARE) {
        V.x[i][j] = UNVISITED;
      } else {
        V.x[i][j] = WALL_SQUARE;
      }
    }

  }

  return V;
}


Matrix getPathMatrix(Matrix *M) {
  Matrix P = initMatrix(M->nrow, M->ncol);

  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] == WALL_SQUARE) P.x[i][j] = WALL_SQUARE;
      else if (M->x[i][j] == START_SQUARE) P.x[i][j] = 0;
      else P.x[i][j] = INT_MAX;
    }
  }

  return P;
}


Matrix readfile(const char *path) {
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    printf("Unable to open file!\n");
    exit(-1);
  }

  int ncol = 0, nrow = 0;

  char c;
  while ((c = getc(file)) != EOF) {
    if (!nrow && c != '\n') ncol++;
    else if (c == '\n')     nrow++;
  }

  printf("nrow = %d, ncol = %d\n", nrow, ncol);
  Matrix M = initMatrix(nrow, ncol);
     

  // reset
  fseek(file, 0L, SEEK_SET);

  int i = 0, j = 0;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      j = 0;
      i++;
    } else {
      switch (c) {
        case '#':
          M.x[i][j++] = WALL_SQUARE;
          break;
        case '.':
          M.x[i][j++] = EMPTY_SQUARE;
          break;
        case 'S':
          M.x[i][j++] = START_SQUARE;
          break;
        case 'E':
          M.x[i][j++] = END_SQUARE;
          break;      
      }
    }

  }


  fclose(file);

  return M;
}


int isValidSquare(int i, int j, Matrix *M) {
  return i >= 0 && i < M->nrow && j >= 0 && j < M->ncol &&
    M->x[i][j] != WALL_SQUARE;
}


int isEndSquare(int i, int j, Matrix *M) {
  return i >= 0 && i < M->nrow && j >= 0 && j < M->ncol &&
    M->x[i][j] == END_SQUARE;
}


int Djikstra2t3(Matrix *M, Matrix *V, Matrix *P) {
  int i0, j0, d = INT_MAX;
 
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] == WALL_SQUARE || 
          V->x[i][j] == VISITED ||
          P->x[i][j] >= d) continue;
      i0 = i, j0 = j, d = P->x[i][j];
      break;
    }
  }

  if (d == INT_MAX) return 0; // done

  V->x[i0][j0] = VISITED;


  int di[4] = {0, 0, 1, -1};
  int dj[4] = {1, -1, 0, 0};


  for (int k = 0; k < 4; k++) {
    int i1 = i0 + di[k], j1 = j0 + dj[k];
    
    if (isValidSquare(i1, j1, M) && P->x[i0][j0] + 1 < P->x[i1][j1])
      P->x[i1][j1] = P->x[i0][j0] +1;
  }

  return 1; // still going
}


void Djikstra(Matrix *M, Matrix *V, Matrix *P) {
  while (Djikstra2t3(M, V, P));
}


int isInPath(int i0, int j0, int end_i, int end_j, Matrix *M, Matrix *P) {
  if (end_i == i0 && end_j == j0) return 1;
  else if (P->x[i0][j0] < 0) return 0;

  int di[4] = {0, 0, 1, -1};
  int dj[4] = {1, -1, 0, 0};

  for (int k = 0; k < 4; k++) {
    int i1 = i0 + di[k], j1 = j0 + dj[k];
    
    if (isValidSquare(i1, j1, M) && P->x[i0][j0] + 1 == P->x[i1][j1] &&
    //   printf("[%d,%d]%d->[%d,%d]%d\n", i0, j0, P->x[i0][j0], i1, j1, P->x[i1][j1]);
    
      isInPath(i1, j1, end_i, end_j, M, P)) return 1;
  }

  return 0;
}

int cheat(
    int i0, int j0, 
    int i_ni, int j_ni, 
    Matrix *M, Matrix *P, 
    int threshold, 
    int state, // 0, 1, 2
    int ni, // number of cheats
    int n
    ) {

  if ((i_ni < 0 || i_ni >= M->nrow || j_ni < 0 || j_ni >= M->ncol) ||
      ni > n || !state && M->x[i_ni][j_ni] == WALL_SQUARE ||
      M->x[i_ni][j_ni] == VISITED) 
    return 0;

  int di[4] = {0, 0, 1, -1};
  int dj[4] = {1, -1, 0, 0};

  int sum = 0, nextState = 0;
  if (!state) {
    nextState = 1;

  } else if (state == 1) {
    if (M->x[i_ni][j_ni] != WALL_SQUARE) {
      M->x[i_ni][j_ni] = VISITED;
      // printf("ni = %d, i_ni = %d, j_ni = %d, P->x[i_ni][j_ni] = %d\n", ni, i_ni, j_ni, P->x[i_ni][j_ni]);
      return P->x[i0][j0] + ni + threshold <= P->x[i_ni][j_ni];
    }

    nextState = 1;
  }

  Matrix N = copyMatrix(M);
  N.x[i_ni][j_ni] = VISITED;
  for (int k = 0; k < 4; k++) {
    int i_np = i_ni + di[k], j_np = j_ni + dj[k];
    sum += cheat(i0, j0, i_np, j_np, M, P, threshold, nextState, ni + 1, n);
  }
  freeMatrix(&N);

  return sum;
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    exit(-1);
  }

  Matrix M = readfile(argv[1]);
  Matrix V = getVisitedMatrix(&M);
  Matrix P = getPathMatrix(&M);

  int end_i, end_j;
  for (int i = 0; i < M.nrow; i++) for (int j = 0; j < M.ncol; j++) {
    if (!isValidSquare(i, j, &M) || M.x[i][j] != END_SQUARE) 
      continue;

    end_i = i, end_j = j;
    break;
  }

  // printf("M:\n");
  // printMap(&M);
  // printf("V:\n");
  // printMatrix(&V);
  // printf("P:\n");
  // printMatrix(&P);

  Djikstra(&M, &V, &P);
  int base = P.x[end_i][end_j];
  printf("base = %d\n", base);
  freeMatrix(&V);
  // freeMatrix(&P);
  
  
  // Task 1
  int sum_t1 = 0, threshold = 64;
  for (int i0 = 0; i0 < M.nrow; i0++) for (int j0 = 0; j0 < M.ncol; j0++) {
    if (!isValidSquare(i0, j0, &M)) 
      continue;

    int p0 = P.x[i0][j0];

    int di[4] = {0, 0, 1, -1};
    int dj[4] = {1, -1, 0, 0};

    for (int k = 0; k < 4; k++) {
      int i1 = i0 + di[k], j1 = j0 + dj[k];
      
      if (isValidSquare(i1, j1, &M))
        continue;
    
      for (int l = 0; l < 4; l++) {
        int i2 = i1 + di[l], j2 = j1 + dj[l];
        
        if (!isValidSquare(i2, j2, &M))
          continue;
      
        int p2 = P.x[i2][j2];
  
        if (p2 + threshold + 2 <= p0) {
          // printMatrix(&P);
          // printf("P0[%d,%d]=%d, P2[%d,%d]=%d\n", i0,j0,p0, i2,j2,p2);
          sum_t1++;
        }
      }
    }
  }
  
  // Task 1
  sum_t1 = 0;
  for (int i0 = 0; i0 < M.nrow; i0++) for (int j0 = 0; j0 < M.ncol; j0++) {
    if (!isValidSquare(i0, j0, &M)) 
      continue;
    
    Matrix N = copyMatrix(&M);

    sum_t1 += cheat(i0, j0, i0, j0, &N, &P, 64, 0, 0, 2);
    freeMatrix(&N);
  }
  
  printf("\nSum = %d\n", sum_t1);
  // Task 1
  int sum_t2 = 0;
  for (int i0 = 0; i0 < M.nrow; i0++) for (int j0 = 0; j0 < M.ncol; j0++) {
    if (!isValidSquare(i0, j0, &M)) 
      continue;
    
    Matrix N = copyMatrix(&M);

    sum_t2 += cheat(i0, j0, i0, j0, &N, &P, 64, 0, 0, 100);
    freeMatrix(&N);
  }


  printf("\nSum = %d\n", sum_t2);
  freeMatrix(&M);
  freeMatrix(&P);

  return 0;
}
