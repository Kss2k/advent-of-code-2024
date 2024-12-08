#include "shared.h"

#define MAX_LOOP 100000


void placeAntenaT1(int i, int j, Map *T1) {
  if (
    i < 0 || i >= T1->nrow ||
    j < 0 || j >= T1->ncol 
  ) return;

  T1->x[i][j] = '#';
}


void placeAntenaT2(int i0, int j0, int i1, int j1, Map *T2) {
  int step_i = i1 - i0;
  int step_j = j1 - j0;

  // forwards
  int i = i0, j = j0;
  while (i >= 0 && i < T2->nrow && j >= 0 && j < T2->ncol) {
    T2->x[i][j] = '#';
    i += step_i;
    j += step_j;
  }

  // backwards
  i = i0, j = j0;
  while (i >= 0 && i < T2->nrow && j >= 0 && j < T2->ncol) {
    T2->x[i][j] = '#';
    i -= step_i;
    j -= step_j;
  }
}


int main(int argv, char **argc) {
  if (argv <= 1) {
    printf("Missing File arg!");
    return -1;
  }

  Map *M = readfile(argc[1]);
  Map *T1 = copyMap(M);
  Map *T2 = copyMap(M);

  Array antenas = initArray(INITIAL_CAPACITY);

  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] == '.' || isin(M->x[i][j], antenas)) continue;
      append(&antenas, M->x[i][j]);
    }
  }

  IntArray I = initIntArray(INITIAL_CAPACITY);
  IntArray J = initIntArray(INITIAL_CAPACITY);
  for (int a = 0; a < antenas.length; a++) {
    getLocations(antenas.x[a], M, &I, &J);

    for (int l0 = 0; l0 < I.length; l0++) {
      int i0 = I.x[l0];
      int j0 = J.x[l0];

      for (int l1 = 0; l1 < I.length; l1++) {
        if (l1 == l0) continue;
        int i1 = I.x[l1];
        int j1 = J.x[l1];
       
        placeAntenaT1(i0 - (i1 - i0), j0 - (j1 - j0), T1);
        placeAntenaT2(i0, j0, i1, j1, T2);
      }
    }

    resetIntArray(&I);
    resetIntArray(&J);
  }

  printf("Answer Task 1\n  Sum = %d\n", countOccurences('#', T1));
  printf("Answer Task 2\n  Sum = %d\n", countOccurences('#', T2));

  freeMap(M);
  freeMap(T1);
  freeMap(T2);
  freeArray(&antenas);
  freeIntArray(&I);
  freeIntArray(&J);

  return 0;
}
