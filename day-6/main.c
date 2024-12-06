#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOOP 50000

typedef struct {
  char **x;
  int nrow;
  int ncol;
} Map;


Map *initMap(int nrow, int ncol) {
  Map *M = (Map*)malloc(sizeof(Map));
  M->nrow = nrow;
  M->ncol = ncol;

  M->x = (char**)malloc(nrow * sizeof(*M->x));
  for (int i = 0; i < nrow; i++) {
    M->x[i] = (char*)malloc(ncol * sizeof(char));
  }

  return M;
}


Map *copyMap(Map *M) {
  Map *N = initMap(M->nrow, M->ncol);
  for (int i = 0; i < M->nrow; i++) {
    memcpy(N->x[i], M->x[i], M->ncol * sizeof(M->x[i][0]));
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


int isOk(char c) {
  return c == '.' || c == 'X' || c == 'v' || c == '^' || c == '<' || c == '>' || c == '#';
}


void printMap(Map *M) {
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (!isOk(M->x[i][j])) printf("\nFOUND character (%d) where it should not be!\n", M->x[i][j]);
      printf("%c", M->x[i][j]);
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
    M->x[i][j++] = c;
  }
  
  fclose(file);

  return M; 
}


int isGuard(char c) {
  return c == '^' || c == '<' || c == '>' || c == 'v';
}


int isHindered(int i, int j, Map *M) {
  switch (M->x[i][j]) {
    case '^': {
      if (i <= 0) return 0;
      return M->x[i - 1][j] == '#';
    }
    case '<': {
      if (j <= 0) return 0;
      return M->x[i][j - 1] == '#';
    }
    case '>': {
      if (j + 1 >= M->ncol) return 0;
      return M->x[i][j + 1] == '#';
    }
    case 'v': {
      if (i + 1 >= M->nrow) return 0;
      return M->x[i + 1][j] == '#';
    }
  }
  printf("Tried to check if something else than guard (%c) was hindered!\n", M->x[i][j]);
  return -1;
}


void rotateGuard(int i, int j, Map *M) {
  switch (M->x[i][j]) {
    case '^': {
      M->x[i][j] = '>';
      return;
    }
    case '>': {
      M->x[i][j] = 'v';
      return;
    }
    case 'v': {
      M->x[i][j] = '<';
      return;
    }
    case '<': {
      M->x[i][j] = '^';
      return;
    }
  }
  printf("Tried to rotate something else than guard (%c)!\n", M->x[i][j]);
  return;
}


int moveGuard(int i, int j, Map *M, int trail) {
  char c = M->x[i][j];
  if (trail) M->x[i][j] = 'X';
  else M->x[i][j] = '.';

  switch (c) {
    case '^': {
      if (i <= 0) return 0;
      M->x[i - 1][j] = '^';
      return 1;
    }
    case '<': {
      if (j <= 0) return 0;
      M->x[i][j - 1] = '<';
      return 1;
    }
    case '>': {
      if (j + 1 >= M->ncol) return 0;
      M->x[i][j + 1] = '>';
      return 1;
    }
    case 'v': {
      if (i + 1 >= M->nrow) return 0;
      M->x[i + 1][j] = 'v';
      return 1;
    }
  }

  printf("Tried to move something else than guard (%c)!\n", c);
  printMap(M);
  return -1;
}


int countX(Map *M) {
  int sum = 0;
  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (M->x[i][j] == 'X') sum++;
    }
  }
  return sum;
}


int iterMap(Map *M, int trail) {
  int found = 0;

  for (int i = 0; i < M->nrow; i++) {
    for (int j = 0; j < M->ncol; j++) {
      if (!isGuard(M->x[i][j])) continue;
      found = 1;
      // printf("found guard at %d, %d\n", i, j);
      if (isHindered(i, j, M)) {
        // printf("Guard is hindered!\n");
        rotateGuard(i, j, M);
      } else {
        // printf("Guard is not hindered!\n");
        found = moveGuard(i, j, M, trail);
      }

      return found;
    }
  }

  return found;
}


void findGuard(int *i, int *j, Map *M) {
  for (*i = 0; *i < M->nrow; (*i)++) {
    for (*j = 0; *j < M->ncol; (*j)++) {
      if (isGuard(M->x[*i][*j])) return;
    }
  }
  printf("Could not find guard!\n");
  return;
}


int checkIfLoop(Map *M) {
  Map *N = copyMap(M);

  int g_i, g_j, loop = 0;
  for (int i = 0; i < MAX_LOOP && iterMap(M, 0); i++) {
    printf("Iteration %d\r", i);

    findGuard(&g_i, &g_j, M);
    if (N->x[g_i][g_j] == M->x[g_i][g_j]) {
      loop = 1;
      break;
    } else N->x[g_i][g_j] = M->x[g_i][g_j];

    if (i + 1 == MAX_LOOP) printf("\nWARNING: REACED MAX!\n");
  }

  freeMap(N);

  return loop;
}


int main(int argv, char **argc) {
  if (argv <= 1) {
    printf("Missing File arg!");
    return -1;
  }

  Map *M = readfile(argc[1]);
  Map *N = copyMap(M);

  int i; 
  for (i = 0; i < MAX_LOOP; i++) {
    // printf("Iteration %d\n", i);
    int status = iterMap(M, 1);
    // printMap(M);
    if (!status) break;
  }

  if (i == MAX_LOOP) {
    printf("Max loop reached!\n");
  } else {
    printf("Answer task 1:\n  Sum = %d\n", countX(M));
  }

  Map *O;
  printMap(N);

  // Task 2
  int sum_t2 = 0;
  for (i = 0; i < N->nrow; i++) {
    for (int j = 0; j < N->ncol; j++) {
      // if (N->x[i][j] == '#' || isGuard(N->x[i][j])) continue;
      if (M->x[i][j] != 'X') continue;

      Map *O = copyMap(N);
      O->x[i][j] = '#';
      printf("Checking if hinderance at %d,%d\n", i, j);
      // printMap(N);
      int isLoop = checkIfLoop(O);
      if (isLoop) sum_t2++;

      freeMap(O);
    }
  }

  printf("Answer Task 2:\n  Sum = %d\n", sum_t2);

  freeMap(M);
  freeMap(N);

  return 0;
}
