#include "shared.h"


int main(int argv, char *argc[]) {
  if (argv < 2) {
    printf("Expected a file input!\n");
    return -1;
  }
  
  Arrays X = readfile(argc[1]);
  
  // printArray(X.before);
  // printArray(X.after);
  // printArrayArray(X.orders);

  int sum_t1 = 0;
  int sum_t2 = 0;
  for (int i = 0; i < X.orders.length; i++) {
    if (isOrderValid(X.orders.x[i], X.before, X.after)) {
      sum_t1 += getMiddleNumber(X.orders.x[i]);
    } else {
      fixOrder(&X.orders.x[i], &X.before, &X.after);
      sum_t2 += getMiddleNumber(X.orders.x[i]);
    }
  }

  printf("Answer task 1:\n  Sum = %d\n", sum_t1);
  printf("Answer task 2:\n  Sum = %d\n", sum_t2);

  freeArray(&X.before);
  freeArray(&X.after);
  freeArrayArray(&X.orders);

  return 0;
}
