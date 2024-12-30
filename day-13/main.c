#include "shared.h"


#define SHIFT 10000000000000
#define COUNT_TOKENS(machines, sum, shifted) \
  do { \
    sum = 0; \
    for (int i = 0; i < machines.length; i++) { \
      long int price = getLowestPrice(&machines.x[i], shifted); \
      if (price != -1) { \
        sum += price; \
      } \
    } \
  } while(0)


typedef struct {
  double x[2][2];
} TwoDMatrix;


void printTDMatrix(TwoDMatrix *M) {
  printf("[%.2f %.2f]\n", M->x[0][0], M->x[0][1]);
  printf("[%.2f %.2f]\n", M->x[1][0], M->x[1][1]);
  printf("\n");
}


typedef struct {
  double x[2];
} TwoDVector;


void printTDVector(TwoDVector *x) {
  printf("[%.2f %.2f]\n", x->x[0], x->x[1]);
}


TwoDMatrix matrix(ClawMachine *machine) {
  TwoDMatrix M;
  M.x[0][0] = machine->A.x;
  M.x[0][1] = machine->B.x;
  M.x[1][0] = machine->A.y;
  M.x[1][1] = machine->B.y;

  return M;
}


TwoDVector vector(ClawMachine *machine) {
  TwoDVector x;
  x.x[0] = machine->prize.x;
  x.x[1] = machine->prize.y;

  return x;
}


TwoDMatrix adjoint(TwoDMatrix *M) {
  TwoDMatrix A;
  A.x[0][0] =  M->x[1][1];
  A.x[1][1] =  M->x[0][0];
  A.x[1][0] = -M->x[1][0];
  A.x[0][1] = -M->x[0][1];

  return A;
}


TwoDMatrix timesScalar(TwoDMatrix *M, double c) {
  TwoDMatrix A;
  A.x[0][0] = c * M->x[0][0];
  A.x[0][1] = c * M->x[0][1];
  A.x[1][0] = c * M->x[1][0];
  A.x[1][1] = c * M->x[1][1];

  return A;
}


TwoDVector timesVector(TwoDMatrix *M, TwoDVector *x) {
  TwoDVector y;
  y.x[0] = x->x[0] * M->x[0][0] + x->x[1] * M->x[0][1];
  y.x[1] = x->x[0] * M->x[1][0] + x->x[1] * M->x[1][1];

  return y;
}


TwoDMatrix inv(TwoDMatrix *M) {
  double det = M->x[0][0] * M->x[1][1] - M->x[0][1] * M->x[1][0];
  TwoDMatrix A = adjoint(M);

  return timesScalar(&A, 1 / det);
}


double dabs(double x) {
  return x < 0 ? -x : x;
}


int isint(TwoDVector *x, double tol) {
  return dabs(x->x[0] - round(x->x[0])) < tol && 
    dabs(x->x[1] - round(x->x[1])) < tol;
}


int parseNumber(FILE *file) {
  char c;
  int num = 0, sign = 1, i = 0;

  while ((c = getc(file)) != ' ' && c != '\n' && c != EOF) {
    if (c == '-') {
      sign = -1;
      continue;

    } else if (c == '+' || c == '=') {
      sign = 1;
      continue;

    } else if (!isdigit(c)) break;

    num = num * 10 + (c - '0');
  }

  fseek(file, -1, SEEK_CUR);

  return sign * num;
}


ClawMachine parseClawMachine(FILE *file) {
  ClawMachine machine;

  char state = 'A', c;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      switch (state) {
        case 'A': 
          state = 'B';
          break;
        case 'B':
          state = 'P';
          break;
        case 'P':
          return machine;
      }
      
      continue;
    }
  

    if (c == 'X' || c == 'Y') {
      long long num = parseNumber(file);
      switch (state) {
        case 'A':
          if (c == 'X') machine.A.x = num;
          else machine.A.y = num;
          break;
        case 'B':
          if (c == 'X') machine.B.x = num;
          else machine.B.y = num;
          break;
        case 'P':
          if (c == 'X') machine.prize.x = num;
          else machine.prize.y = num;
          break;
      }
    }
  }

  return machine;
}


void printMachine(ClawMachine *machine) {
  printf("Button A: X=%lld, Y=%lld\n", machine->A.x, machine->A.y);
  printf("Button B: X=%lld, Y=%lld\n", machine->B.x, machine->B.y);
  printf("prize: X=%lld, Y=%lld\n", machine->prize.x, machine->prize.y);
}


Vector readfile(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  char c;
  Vector machines = initVector(INITIAL_CAPACITY);
  while ((c = getc(file)) != EOF) {
    if (c == '\n') continue;
    ClawMachine machine = parseClawMachine(file);
    append(&machines, machine); 
  }

  fclose(file);
  return machines;
}


long long getLowestPrice(ClawMachine *machine, int shifted) {
  TwoDMatrix M = matrix(machine);
  TwoDMatrix Mi = inv(&M);
  TwoDVector y = vector(machine);
  TwoDVector b = timesVector(&Mi, &y);

  if (!isint(&b, 0.01)) return -1;

  return 3 * round(b.x[0]) + round(b.x[1]);
}


void setNewPos(ClawMachine *machine, long long shift) {
  machine->prize.x += shift;
  machine->prize.y += shift;
}


void setNewPositions(Vector *machines, long long shift) {
  for (int i = 0; i < machines->length; i++)
    setNewPos(&machines->x[i], shift);
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    return -1;
  }
  
  Vector machines = readfile(argv[1]);
  long long sum_t1, sum_t2;
  COUNT_TOKENS(machines, sum_t1, 0); 
  printf("Sum of lowest prices: %lld\n", sum_t1);

  setNewPositions(&machines, 10000000000000);
  COUNT_TOKENS(machines, sum_t2, 1);
  printf("Sum of lowest prices after shift: %lld\n", sum_t2);

  freeVector(&machines);
  return 0;
}
