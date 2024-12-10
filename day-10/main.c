#include "shared.h"



int trailheadT1(const int i0, const int j0, Map *M) {
  if (M->x[i0][j0] == 9) {
    M->x[i0][j0] = -99;
    return 1;
  }

  int steps[2] = {-1, 1};

  int trailheads = 0;
  for (int i = 0; i < 2; i++) {
    int i1 = i0 + steps[i];

    int valid_step = 
      i1 >= 0 && i1 < M->nrow && 
      M->x[i1][j0] == M->x[i0][j0] + 1;

    if (!valid_step) continue;

    trailheads += trailheadT1(i1, j0, M);
  }

  for (int j = 0; j < 2; j++) {
    int j1 = j0 + steps[j];

    int valid_step = 
      j1 >= 0 && j1 < M->ncol && 
      M->x[i0][j1] == M->x[i0][j0] + 1;

    if (!valid_step) continue;

    trailheads += trailheadT1(i0, j1, M);
  }

  return trailheads;
}


int trailheadT2(const int i0, const int j0, Map *M) {
  if (M->x[i0][j0] == 9) return 1;

  int steps[2] = {-1, 1};

  int trailheads = 0;
  for (int i = 0; i < 2; i++) {
    int i1 = i0 + steps[i];

    int valid_step = 
      i1 >= 0 && i1 < M->nrow && 
      M->x[i1][j0] == M->x[i0][j0] + 1;

    if (!valid_step) continue;

    trailheads += trailheadT2(i1, j0, M);
  }

  for (int j = 0; j < 2; j++) {
    int j1 = j0 + steps[j];

    int valid_step = 
      j1 >= 0 && j1 < M->ncol && 
      M->x[i0][j1] == M->x[i0][j0] + 1;

    if (!valid_step) continue;

    trailheads += trailheadT2(i0, j1, M);
  }

  return trailheads;
}



int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file arg!\n");
    exit(-1);
  }

  Map *M = readfile(argv[1]);
 
  int sum_t1 = 0, sum_t2 = 0;
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      // if (i != 0 || j != 2) continue;
      if (M->x[i][j] != 0) continue;
      Map *N = copyMap(M);
      sum_t2 += trailheadT2(i, j, N);
      sum_t1 += trailheadT1(i, j, N);

      freeMap(N);
    }
  }

  printf("Answer Task 1:\n  Sum = %d\n", sum_t1);
  printf("Answer Task 2:\n  Sum = %d\n", sum_t2);

  freeMap(M);

  return 0;
}
