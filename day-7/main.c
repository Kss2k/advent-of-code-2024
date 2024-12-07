#include "shared.h" 


int matchTargetTask1(Array *X, long long int target) {
  if (X->size == 1) return X->x[0] == target;
  if (X->size == 2) return X->x[0] * X->x[1] == target || X->x[0] + X->x[1] == target;
  Array Y = initArrayN(X->size - 1);

  // check *
  append(&Y, X->x[0] * X->x[1]);
  for (int i = 1; i < X->size - 1; i++) append(&Y, X->x[i + 1]);

  if (matchTargetTask1(&Y, target)) return 1;

  // Check +
  Y.x[0] = X->x[0] + X->x[1];

  return matchTargetTask1(&Y, target);
}


long long int get10s(long long int x) {
  if (x == 0) return 0;
  long long tens = 1;
  for (int i = 1; i < MAX_10S; i++) {
    tens = tens * 10;
    if (x < tens) return tens;
  }
  printf("WARNING Reached max in get10s!\n");
  return tens;
}


int matchTargetTask2(Array *X, long long int target) {
  if (X->size == 1) return X->x[0] == target;
  if (X->size == 2) return 
    X->x[0] * X->x[1] == target || 
    X->x[0] + X->x[1] == target  ||
    X->x[0] * get10s(X->x[1]) + X->x[1] == target ; Array Y = initArrayN(X->size - 1);

  // check *
  append(&Y, X->x[0] * X->x[1]);
  for (int i = 1; i < X->size - 1; i++) append(&Y, X->x[i + 1]);

  if (matchTargetTask2(&Y, target)) return 1;

  // Check +
  Y.x[0] = X->x[0] + X->x[1];
  if (matchTargetTask2(&Y, target)) return 1;

  // check ||
  Y.x[0] = X->x[0] * get10s(X->x[1]) + X->x[1];

  return matchTargetTask2(&Y, target);
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file-argument!\n");
    exit(-1);
  }

  FILE *file = fopen(argv[1], "r");

  char c;
  long long int num = 0, onum = 0, target, sum_t1 = 0, sum_t2 = 0;
  Array numbers = initArray();
  
  while ((c = getc(file)) != EOF) {
    if (isdigit(c)) {
      num = num * 10 +  c - '0';
      onum = 1;
      continue;
    } 
    
    switch (c) {
      case ':': {
        if (onum) target = num;
        break;
      }
      case ' ': {
        if (onum) append(&numbers, num);
        break;
      }
      case '\n': {
        if (onum) append(&numbers, num);
        if (matchTargetTask1(&numbers, target)) {
          // mpz_add_ui(sum_t1, sum_t1, target);
          sum_t1 += target;
        }
        if (matchTargetTask2(&numbers, target)) {
          // mpz_add_ui(sum_t1, sum_t1, target);
          sum_t2 += target;
        }
        resetArray(&numbers);
        target = 0;
        break;
      }
    }

    num = 0;
    onum = 0;

  }

  printf("Answer task 1:\n  Sum %lld\n", sum_t1);
  printf("Answer task 2:\n  Sum %lld\n", sum_t2);

  free(numbers.x);
  fclose(file);

  return 0;
}
