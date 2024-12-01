#include <stdio.h>
#include "shared.h"
#include "stdlib.h"


int comp(const void *x, const void *y) {
  return *(int *)x - *(int *)y; // cast to int pointer, then cast to int
}


int abs(int x) {
  return x < 0 ? -x : x;
}


int main(int argc, char **argv) {
  if (argc < 2) {
    printf("missing input!\n");
    return -1;
  }

  // READ FILE, GET- AND CHECK ARRAYS
  TwoArrays arrays = readFileAllocateArrays(argv[1]);
  qsort(arrays.x, arrays.length_x, sizeof(arrays.x[0]), comp);
  qsort(arrays.y, arrays.length_y, sizeof(arrays.y[0]), comp);

  if (arrays.length_y != arrays.length_x) printf("unequal lengths!");

  // TASK 1:
  int sum_t1 = 0;
  for (int i = 0; i < arrays.length_x; i++) {
    sum_t1 += abs(arrays.x[i] - arrays.y[i]); 
  }
  printf("ANSWER TASK 1\n  The sum is %d\n", sum_t1);


  // TASK 2:
  int j = 0; // This is simplified by the fact that the lists are sorted
  int previous;
  int occurences;
  int sum_t2; 
  for (int i = 0; i < arrays.length_x; i++) {
    // if current == previous just use the last result
    if (i && arrays.x[i] == previous) {
      sum_t2 += occurences * previous; 
      continue;
    } 

    // find first match
    while (arrays.y[j] < arrays.x[i] && j < arrays.length_y) j++;


    // clean up if we overshoot (not finding any match)
    if (arrays.y[j] > arrays.x[i]) { 
      if (j) j--;
      continue;
    }

    // count occurences
    occurences = 0;
    for (; j < arrays.length_y && arrays.y[j] == arrays.x[i]; j++) {
      occurences++; 
    }

    sum_t2 += occurences * arrays.x[i];
    previous = arrays.x[i];
  }

  printf("ANSWER TASK 2\n  The sum is %d\n", sum_t2);
  return 0;
}
