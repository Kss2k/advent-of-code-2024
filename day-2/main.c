#include "shared.h"


int main(int argv, char *argc[]) {
  if (argv < 2) {
    printf("Expected a file input!\n");
    return -1;
  }

  FILE *file = fopen(argc[1], "r");

  if (file == NULL) {
    printf("Could not open file!\n");
    return -1;
  }

  Stack stack = initStack();

  
  // Task 1 --------------------------------------------------------------------
  int sum_t1 = 0;
  char c;
  int num = 0;
  int on_number = 0;
  int on_level = 0;
  int done_level = 0;
  int sign = 0;
  int valid = 0;

  fseek(file, 0L, SEEK_SET);
  while ((c = fgetc(file)) != EOF) {
    if ((c == ' ' || c == '\n') && on_number && !done_level) {
      if (!on_level) {
        put(&stack, num);
        num = 0;
        on_number = 0;
        on_level = 1;

      } else if (isValidDiff(pop(&stack), num, &sign)) {
        put(&stack, num);
        valid = 1;
        num = 0;

      } else {
        done_level = 1;
        valid = 0;
      }

    } else if (isdigit(c)) {
      on_number = 1;
      num = num * 10 + (int)(c - '0');
    }

    if (c == '\n') {
      sum_t1 += valid;
      num = 0;
      on_number = 0;
      on_level = 0;
      done_level = 0;
      sign = 0;
      valid = 0;
      resetStack(&stack);
    }
  }
  printf("Answer task 1:\n  Sum = %d\n", sum_t1);


  // Task 2 --------------------------------------------------------------------
  int sum_t2 = 0;
  num = 0;
  on_number = 0;
  sign = 0;
  valid = 1;
  on_level = 0;

  fseek(file, 0L, SEEK_SET);
  while ((c = fgetc(file)) != EOF) {
    if ((c == ' ' || c == '\n') && on_number) {
      if (on_level && !isValidDiff(top(&stack), num, &sign)) {
        valid = 0;
      }
      on_number = 0;
      on_level = 1;
      put(&stack, num);
      num = 0;

    } else if (isdigit(c)) {
      on_number = 1;
      num = num * 10 + (int)(c - '0');
    }

    if (c == '\n') {
      if (valid) sum_t2++;
      else sum_t2 += canBeValid(&stack);

      num = 0;
      on_number = 0;
      on_level = 0;
      sign = 0;
      valid = 1;
      resetStack(&stack);
    }
  }

  printf("Answer task 2:\n  Sum = %d\n", sum_t2);

  freeStack(&stack);
  fclose(file);

  return 0;
}
