#include "shared.h"
#include <ctype.h>
#include <stdio.h>


void printTDVector(TwoDVector *x) {
  printf("[%.2f %.2f]\n", x->x[0], x->x[1]);
}


TwoDVector vector(int a, int b) {
  TwoDVector x;
  x.x[0] = a;
  x.x[1] = b;

  return x;
}


TwoDVector vectorAddition(TwoDVector *x, TwoDVector *y) {
  TwoDVector z;
  z.x[0] = x->x[0] + y->x[0];
  z.x[1] = x->x[1] + y->x[1];

  return z;
}


void parseVector(FILE *file, Guard *guard, char state) {
  char c;

  int sign = 1, num = 0, pos = 0;
  while ((c = getc(file)) != EOF && pos < 2) {
    if (c == '-') sign = -1;
    else if (!isdigit(c)) {
      switch (state) {
        case 'p': {
          guard->position.x[pos++] = sign * num;
          num = 0;
          break;
        }
        case 'v': {
          guard->velocity.x[pos++] = sign * num;
          num = 0;
          break;
        }
      }
    } else num = 10 * num + c - '0'; 
  }

  fseek(file, -1, SEEK_CUR);
}


Guard parseGuard(FILE *file) {
  TwoDVector position, velocity;
  Guard guard;
  guard.position = position, guard.velocity = velocity;

  char c;
  while ((c = getc(file) != '=') && c != EOF);
  if (c == EOF) return guard;
  parseVector(file, &guard, 'p');

  while ((c = getc(file) != '=') && c != EOF);
  if (c == EOF) return guard;

  parseVector(file, &guard, 'v');

  return guard;
}


Vector readfile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  Vector guards = initVector(INITIAL_CAPACITY);
  char c, state = '\0';
  while ((c = getc(file)) != EOF) {
    if (c != 'p') continue;
    Guard guard = parseGuard(file);
    append(&guards, guard);
  }

  fclose(file);

  return guards;
}



void printGuard(Guard *guard) {
  printf("v=%ld,%ld p=%ld,%ld \n", 
      guard->position.x[0], 
      guard->position.x[1], 
      guard->velocity.x[0],
      guard->velocity.x[1]);
}



int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    return -1;
  } 
  
  Vector guards = readfile(argv[1]);
  printGuard(&guards.x[0]);
 
  
  freeVector(&guards);

  return 0;
}
