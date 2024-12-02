#include "shared.h"


int getSign(int x) {
  return x >= 0 ? 1: -1;
}


int isValidDiff(int x0, int x1, int *sign) {
  int diff = x1 - x0;
  if (*sign == 0) *sign = getSign(diff);

  diff = (*sign) * diff;
  return diff >= 1 && diff <= 3;
}


int canBeValid(Stack *stack) {
  for (int i = 0; i < stack->size; i++) {
    int valid = 1;
    int sign = 0;
    int prev;
    int n = 0;
      
    for (int j = 0; j < stack->size; j++) {
      if (j == i || ++n <= 1) continue;
      if (j - 1 == i) prev = j - 2;
      else prev = j - 1;
      
      valid = valid && n > 1 ? isValidDiff(stack->x[prev], stack->x[j], &sign) : 0;
    }

    if (valid) return 1;
  }

  return 0;
}
