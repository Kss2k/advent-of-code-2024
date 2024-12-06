#include "shared.h"

#define MAX_LOOP 100000

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


int findGuard(int *i, int *j, Map *M) {
  for (*i = 0; *i < M->nrow; (*i)++) {
    for (*j = 0; *j < M->ncol; (*j)++) {
      if (isGuard(M->x[*i][*j])) return 1;
    }
  }
  printf("Could not find guard!\n");
  return 0;
}


int checkIfLoop(Map *M) {
  Map *N1 = copyMap(M); // UGLY AS F**K
  Map *N2 = copyMap(M); // UGLY AS F**K
  Map *N3 = copyMap(M); // UGLY AS F**K

  int g_i, g_j, loop = 0;
  for (int i = 0; i < MAX_LOOP && iterMap(M, 0); i++) {
    printf("  sub-iters: %d\r", i);

    if(!findGuard(&g_i, &g_j, M)) break;
    if (N1->x[g_i][g_j] == M->x[g_i][g_j] || 
        N2->x[g_i][g_j] == M->x[g_i][g_j] ||
        N3->x[g_i][g_j] == M->x[g_i][g_j]) {
      loop = 1;
      break;
    } else {
      N3->x[g_i][g_j] = N2->x[g_i][g_j];
      N2->x[g_i][g_j] = N1->x[g_i][g_j];
      N1->x[g_i][g_j] = M->x[g_i][g_j];
    }

    if (i + 1 == MAX_LOOP) {
      printf("\nWARNING: REACHED MAX!\n");
      loop = 1;
    }
  }

  printf("\n");

  freeMap(N1);
  freeMap(N2);
  freeMap(N3);

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
    int status = iterMap(M, 1);
    if (!status) break;
  }

  int sum_t1 = countX(M);
  if (i == MAX_LOOP) {
    printf("Max loop reached!\n");
  } else {
    printf("Answer task 1:\n  Sum = %d\n", sum_t1);
  }

  Map *O;
  printMap(N);

  // Task 2
  int sum_t2 = 0;
  for (i = 0; i < N->nrow; i++) {
    for (int j = 0; j < N->ncol; j++) {
      if (M->x[i][j] != 'X') continue;

      Map *O = copyMap(N);
      O->x[i][j] = '#';
      printf("Checking if hinderance at %d,%d\n", i, j);
      int isLoop = checkIfLoop(O);
      if (isLoop) sum_t2++;

      freeMap(O);
    }
  }
  // Array xs     = initArray(M->nrow * M->ncol);
  // Array ys     = initArray(M->nrow * M->ncol);

  // int sum_t2 = 0, g_j, g_i, x, y;
  // for (i = 0; i < MAX_LOOP; i++) {
  //   if (!iterMap(N, 0)) {
  //     printf("Iter stopped!\n");
  //     break;
  //   }
  //   int found = findGuard(&g_i, &g_j, N);
  //   if (!found) break;
  //   printf("Iteration %d\n", i+1);
  //   Map *O = copyMap(N);
  //   
  //   switch (N->x[g_i][g_j]) {
  //     case '^': {
  //       if (g_i <= 0) break;
  //       x = g_i - 1;
  //       y = g_j;
  //       break;
  //     }
  //     case '<': {
  //       if (g_j <= 0) break;
  //       x = g_i;
  //       y = g_j - 1;
  //       break;
  //     }
  //     case '>': {
  //       if (g_j + 1 >= M->ncol) break;
  //       x = g_i;
  //       y = g_j + 1;
  //       break;
  //     }
  //     case 'v': {
  //       if (g_i + 1 >= M->nrow) break;
  //       x = g_i + 1;
  //       y = g_j;
  //       break;
  //     }
  //   }

  //   if (isin(x, y, xs, ys)) {
  //     printf("Found loop at %d, %d\n", x, y);
  //     sum_t2++;
  //     continue;
  //   }

  //   O->x[y][x] = '#';

  //   int isLoop = checkIfLoop(O);
  //   if (isLoop) {
  //     sum_t2++;
  //     append(&xs, x);
  //     append(&ys, y);
  //   }
 
  //   freeMap(O);
  // }

  printf("Answer Task 2:\n  Sum = %d\n", sum_t2);

  freeMap(M);
  freeMap(N);

  return 0;
}
