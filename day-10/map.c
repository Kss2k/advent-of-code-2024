#include "shared.h"


Map *initMap(int nrow, int ncol) {
  Map *M = (Map*)malloc(sizeof(Map));
  M->nrow = nrow;
  M->ncol = ncol;

  M->x = (MAP_TYPE**)malloc(nrow * sizeof(*M->x));
  for (int i = 0; i < nrow; i++) {
    M->x[i] = (MAP_TYPE*)malloc(ncol * sizeof(MAP_TYPE));
  }

  return M;
}


Map *copyMap(Map *M) {
  Map *N = initMap(M->nrow, M->ncol);
  for (int i = 0; i < M->nrow; i++) {
    memcpy(N->x[i], M->x[i], M->ncol * sizeof(MAP_TYPE));
  }

  return N;
}


void freeMap(Map *M) {
  for (int i = 0; i < M->nrow; i++) {
    free(M->x[i]);
  }
  free(M->x);
  free(M);
}


void printMap(Map *M) {
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      printf("%d", M->x[i][j]);
    }
    printf("\n");
  } 
}


Map *readfile(const char *path) {
  FILE *file = fopen(path, "r"); 
  
  if (file == NULL) {
    printf("Unable to open file!");
    return NULL;
  }

  fseek(file, 0L, SEEK_SET);
  char c;
  int ncol;
  for (ncol = 0; (c = getc(file)) != '\n'; ncol++);

  fseek(file, 0L, SEEK_SET);
  int nrow = 0;
  while ((c = getc(file)) != EOF) if (c == '\n') nrow++;
  
  printf("nrow = %d, ncol = %d\n", nrow, ncol);
  Map *M = initMap(nrow, ncol);

  
  fseek(file, 0L, SEEK_SET);

  int i = 0, j = 0;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      i++;
      j = 0;
      continue;
    }
    M->x[i][j++] = c - '0';
  }
  
  fclose(file);

  return M; 
}


int countOccurences(MAP_TYPE x, Map *M) {
  int sum = 0;
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->nrow; j++) {
      if (M->x[i][j] == x) sum++;
    }
  }
  return sum;
}
