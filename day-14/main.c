#include "shared.h"
#include <ctype.h>
#include <stdio.h>


typedef struct {
  Vector px;
  Vector py;
  Vector vx;
  Vector vy;
} DataFrame;


DataFrame *DF;


DataFrame initDataFrame(const int capacity) {
  DataFrame df;

  df.px = initVector(capacity);
  df.py = initVector(capacity);
  df.vx = initVector(capacity);
  df.vy = initVector(capacity);

  return df;
}


DataFrame copyDataFrame(const *DataFrame df) {
  DataFrame df;

  df.px = copyVector(&df->px);
  df.py = copyVector(&df->py);
  df.vx = copyVector(&df->vx);
  df.vy = copyVector(&df->vy);

  return df;
}


void freeDataFrame(DataFrame *df) {
  freeVector(&df->px);
  freeVector(&df->py);
  freeVector(&df->vx);
  freeVector(&df->vy);
}


void compRow(void *x, void *y) {
  ()
}

void sortDataFrame(DataFrame *df) {
  DF = df;    
}


void parseVector(FILE *file, DataFrame *df, char state) {
  char c, pos = 'x';

  int sign = 1, num = 0;
  while ((c = getc(file)) != EOF && pos != 'e') {
    if (c == '-')
      sign = -1;
    else if (!isdigit(c)) {
      switch (state) {
      case 'p': {
        if (pos == 'x') {
          append(&df->px, sign * num);
          pos = 'y';
        } else {
          append(&df->py, sign * num);
          pos = 'e';
        }

        num = 0;
        break;
      }
      case 'v': {
        if (pos == 'x') {
          append(&df->vx, sign * num);
          pos = 'y';
        } else {
          append(&df->vy, sign * num);
          pos = 'e';
        }

        num = 0;
        break;
      }
      }
    } else
      num = 10 * num + c - '0';
  }

  fseek(file, -1, SEEK_CUR);
}

void parseGuard(FILE *file, DataFrame *df) {

  char c;
  while ((c = getc(file) != '=') && c != EOF)
    ;
  if (c == EOF)
    return;

  parseVector(file, df, 'p');

  while ((c = getc(file) != '=') && c != EOF)
    ;
  if (c == EOF)
    return;

  parseVector(file, df, 'v');

  return;
}

DataFrame readfile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  DataFrame df = initDataFrame(INITIAL_CAPACITY);

  char c, state = '\0';
  while ((c = getc(file)) != EOF) {
    if (c != 'p')
      continue;
    parseGuard(file, &df);
  }

  fclose(file);

  return df;
}


void printDataFrame(DataFrame *df) {
  for (int i = 0; i < df->px.length; i++)
    printf("[,%2d] p.x=%2d, p.y=%2d, v.x=%2d, v.y=%2d\n", i, df->px.x[i],
           df->py.x[i], df->vx.x[i], df->vy.x[i]);
}


void calibrate(Vector *x, const int n) {
  for (int i = 0; i < x->length; i++) {
    x->x[i] = x->x[i] < 0 ? n - (-x->x[i]) % n : x->x[i] % n;
    x->x[i] = x->x[i] == n ? 0 : x->x[i];
  }
}


void add2lhs(Vector *x, Vector *y, const int times) {
  if (x->length != y->length) {
    printf("Error: x and y are non-conformable!\n");
    exit(-1);
  }

  for (int i = 0; i < x->length; i++)
    x->x[i] += y->x[i] * times;
}

void calculate(DataFrame *df, const int n, const int nrow, const int ncol) {
  add2lhs(&df->px, &df->vx, n);
  add2lhs(&df->py, &df->vy, n);

  calibrate(&df->px, ncol);
  calibrate(&df->py, nrow);
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    return -1;
  }

  DataFrame df = readfile(argv[1]);
  printDataFrame(&df);

  freeDataFrame(&df);

  return 0;
}
