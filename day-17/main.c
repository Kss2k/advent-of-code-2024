#include "shared.h"
#include "unistd.h"

long long int to_int(const char *x) {
  int num = 0;
  while (*x) num = 10 * num + *x++ - '0';
  return num;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file arg!\n");
    exit(-1);
  }

  Computer C = readfile(argv[1]);
  Computer V = copyComputer(&C);

  evalProgram(&C);

  printf("Answer Task 1:\n");
  printStdOut(&C);
  freeComputer(&C);

  int equal = 0;

  V.A = pow(8, V.program.length - 1);
  while (!equal && V.A < pow(8, V.program.length)) {
    Computer H = copyComputer(&V);
    evalProgram(&H);
    equal = isequal(&H.program, &H.std_out);

    if (!equal) {
      int k = lastMisMatch(&H.program, &H.std_out);
      V.A += pow(8, k);
    } else {
      printf("Answer Task 2: %lld\n", V.A);
    }
    freeComputer(&H);
  }


  return 0;
}
