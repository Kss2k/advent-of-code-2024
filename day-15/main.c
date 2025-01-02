#include "shared.h"

typedef struct {
  Vector moves;
  Matrix map;
} MapAndMoves;


void freeMapAndMoves(MapAndMoves *M) {
  freeMatrix(&M->map);
  freeVector(&M->moves);
}


void getMapSize(FILE *file, int *nrow, int *ncol) {
  fseek(file, 0, SEEK_SET);
  char c;
  int ncount = 0, col = 0;
  *nrow = 0, *ncol = 0;
  while ((c = fgetc(file)) != EOF && ncount < 2) {
    if (c == '\n') {
      if (!ncount) (*nrow)++;
      ncount++; 
      col = 1;
    } else {
      ncount = 0;
      if (!col) (*ncol)++;
    }
  }

  fseek(file, 0, SEEK_SET);
}


Matrix parseSmallMap(FILE *file) {
  int nrow, ncol, ncount = 0;
  getMapSize(file, &nrow, &ncol);


  Matrix M = initMatrix(nrow, ncol);

  // printf("Map [%d,%d]\n", nrow, ncol);
  char c;
  nrow = 0, ncol = 0;
  while ((c = getc(file)) != EOF && ncount < 2) {
    if (c == '\n') {
      nrow++;
      ncount++; 
      ncol = 0;
    } else {
      M.x[nrow][ncol++] = c;
      ncount = 0;
    }
  }

  fseek(file, -1, SEEK_CUR);

  return M;
}



Matrix parseLargeMap(FILE *file) {
  int nrow, ncol, ncount = 0;
  getMapSize(file, &nrow, &ncol);


  Matrix M = initMatrix(nrow, 2 * ncol);

  char c, c0, c1;
  nrow = 0, ncol = 0;
  while ((c = getc(file)) != EOF && ncount < 2) {
    if (c == '\n') {
      ncount++; 
      nrow++;
      ncol = 0;
    } else {

      switch (c) {
        case '@': {
          c0 = c;
          c1 = '.';
          break;
        }
        case 'O': {
          c0 = '[';
          c1 = ']';
          break;
        }
        case '.': case '#': {
          c0 = c;
          c1 = c;
          break;
        }
      }

      M.x[nrow][ncol++] = c0;
      M.x[nrow][ncol++] = c1;
      ncount = 0;
    }
  }

  fseek(file, -1, SEEK_CUR);

  return M;
}


void printMap(Matrix *M) {
  printf("Map [%ld,%ld]:\n ", M->nrow, M->ncol);
  
  // for (int j = 0; j < M->ncol; j++) 
  //   printf("%d", j % 10);
  // printf("\n");
  for (int i = 0; i < M->nrow; i++) {
    // printf("%d", i);
    for (int j = 0; j < M->ncol; j++) {
      printf("%c", M->x[i][j]);
    }
    printf("\n");
  }
}


void printMapAndMoves(MapAndMoves *M) {
  printf("Map [%ld,%ld]:\n", M->map.nrow, M->map.ncol);
  printMap(&M->map);
  printf("Moves:[%ld, 1]\n", M->moves.length);
  printVector(&M->moves);
}


Vector parseMoves(FILE *file) {
  Vector moves = initVector(INITIAL_CAPACITY);
  char c;
  while ((c = getc(file)) != EOF)
    if (c == 'v' || c == '<' || c == '>' || c == '^')
      append(&moves, c);

  return moves;
}


MapAndMoves readfile(char *filename, int largeMap) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  MapAndMoves M;
  M.map = largeMap ? parseLargeMap(file): parseSmallMap(file);
  M.moves = parseMoves(file);

  fclose(file);

  return M;
}



int moveSmall(MapAndMoves *M, 
         const int i0, 
         const int j0, 
         const char c0, 
         const char m) {

  char c1 = M->map.x[i0][j0];

  if (c1 == '#') 
    return 0;
  else if (c1 == '.') {
    M->map.x[i0][j0] = c0;
    return 1;
  }

  int i1 = i0, j1 = j0;
  switch(m) {
    case 'v': i1++; break;
    case '^': i1--; break;
    case '>': j1++; break;
    case '<': j1--; break;
  }

  if (moveSmall(M, i1, j1, c1, m)) {
    M->map.x[i0][j0] = c0;
    return 1;
  }
  
  return 0;
}


void indent(int n) {
  for (int i = 0; i < n; i++) printf("   ");
}


int moveLarge(MapAndMoves *M, 
         const int i0,
         const int j0,
         const int i1, 
         const int j1, 
         const char m,
         const int depth) {

  char c0 = M->map.x[i0][j0];
  char c1 = M->map.x[i1][j1];

  if (c1 == '#') {
    // indent(depth);
    // printf("BLOCKED, path was directly blocked!");
    // printf(" --> i0: %d, j0: %d, i1: %d, j1: %d, m: %c, c0 = %c, c1 = %c\n", i0, j0, i1, j1, m, c0, c1);
    return 0;
  }
  
  int i2 = i1, j2 = j1;
  switch(m) {
    case 'v': i2++; break;
    case '^': i2--; break;
    case '>': j2++; break;
    case '<': j2--; break;
  }

  if ((c0 == '[' || c0 == ']') && (m == 'v' || m == '^')) {
    int shift = c0 == ']' ? -1: 1;
    int j0b = j0 + shift, j1b = j1 + shift, j2b = j2 + shift;
    char c0b = M->map.x[i0][j0b];
    char c1b = M->map.x[i1][j1b];

    if (c1 == '.' && c1b == '.') {
      M->map.x[i1][j1]  = c0;
      M->map.x[i1][j1b] = c0b;
      M->map.x[i0][j0]  = '.';
      M->map.x[i0][j0b] = '.';

      // indent(depth);
      // printf("MOVED BOX VERTICALLY, path was directly open!");
      // printf(" --> i0: %d, j0: %d, i1: %d, j1: %d, m: %c, c0 = %c, c1 = %c\n", i0, j0, i1, j1, m, c0, c1);
      return 1;
    }

    Matrix N = copyMatrix(&M->map);

    int success = moveLarge(M, i1, j1, i2, j2, m, depth + 1) &&
      moveLarge(M, i0, j0b, i1, j0b, m, depth + 1);

    if (success) {
      M->map.x[i1][j1]  = c0;
      M->map.x[i1][j1b] = c0b;
      M->map.x[i0][j0]  = '.';
      M->map.x[i0][j0b] = '.';

      freeMatrix(&N);
      // indent(depth);
      // printf("MOVED BOX VERTICALLY, path was indirectly open!");
      // printf(" --> i0: %d, j0: %d, i1: %d, j1: %d, m: %c, c0 = %c, c1 = %c\n", i0, j0, i1, j1, m, c0, c1);
      return 1;

    } else {
      freeMatrix(&M->map);
      M->map = N;

      // indent(depth);
      // printf("BLOCKED, path was indirectly blocked!");
      // printf(" --> i0: %d, j0: %d, i1: %d, j1: %d, m: %c, c0 = %c, c1 = %c\n", i0, j0, i1, j1, m, c0, c1);
      return 0;
    }

  } else if (c1 == '.') {
    M->map.x[i1][j1] = c0;
    M->map.x[i0][j0] = '.';
    // indent(depth);
    // printf("MOVED SIMPLE, path was directly open!");
    // printf(" --> i0: %d, j0: %d, i1: %d, j1: %d, m: %c, c0 = %c, c1 = %c\n", i0, j0, i1, j1, m, c0, c1);

    return 1;

  } else if (moveLarge(M, i1, j1, i2, j2, m, depth + 1)) {
    M->map.x[i1][j1] = c0;
    if (i0 != i2 || j0 != j2) {
      M->map.x[i0][j0] = '.';
    }

    // indent(depth);
    // printf("MOVED SIMPLE, path was indirectly open!");
    // printf(" --> i0: %d, j0: %d, i1: %d, j1: %d, i2: %d, j2: %d, m: %c, c0 = %c, c1 = %c\n", i0, j0, i1, j1, i2, j2, m, c0, c1);


    return 1;

  } else {
    return 0;
  }
}


void findGuard(const Matrix *M, int *i0, int *j0) {
  for (int i = 0; i < M->nrow; i++) for (int j = 0; j < M->ncol; j++) {
    if (M->x[i][j] == '@') {
      *i0 = i, *j0 = j;
      return;
    }
  }

  printf("Error: Could not locate guard!\n");
  exit(-9);
}


void iterateSmall(MapAndMoves *M) {
  int i0, j0;

  findGuard(&M->map, &i0, &j0);
  for (int k = 0; k < M->moves.length; k++) {
    int success = moveSmall(M, i0, j0, '.', M->moves.x[k]);

    if (success) {
      switch(M->moves.x[k]) {
        case 'v': i0++; break;
        case '^': i0--; break;
        case '>': j0++; break;
        case '<': j0--; break;
      }
    }

  }
}


void iterateLarge(MapAndMoves *M) {
  int i0, j0, i1, j1;

  findGuard(&M->map, &i1, &j1);
  j0 = j1 + 1;
  i0 = i1;
  for (int k = 0; k < M->moves.length; k++) {
    if (M->map.x[i1][j1] != '@') {
      printf("Error: Guard is missing!\n");
      exit(-9);
    }
    int success = 
      moveLarge(M, i0, j0, i1, j1, M->moves.x[k], 0);

    if (success) {
     
      j0 = j1;
      i0 = i1;

      switch(M->moves.x[k]) {
        case 'v': i1++; break;
        case '^': i1--; break;
        case '>': j1++; break;
        case '<': j1--; break;
      }
    }
  }
}


int gps(Matrix *M, int i, int j) {
  return M->x[i][j] == 'O' || M->x[i][j] == '[' ? 100 * i + j: 0;
}


int sumGPS(Matrix *M) {
  int sum = 0;
  for (int i = 0; i < M->nrow; i++) for (int j = 0; j < M->ncol; j++)
    sum += gps(M, i, j);

  return sum;
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    return -1;
  } 

  // Task 1
  MapAndMoves M = readfile(argv[1], 0);

  printMap(&M.map);

  iterateSmall(&M);

  printf("GPS: %d\n", sumGPS(&M.map));

  freeMapAndMoves(&M);

  // Task 2
  M = readfile(argv[1], 1);
  iterateLarge(&M);

  printMap(&M.map);
  printf("GPS: %d\n", sumGPS(&M.map));

  freeMapAndMoves(&M);
  return 0;
}
