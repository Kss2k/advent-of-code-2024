#include "stdio.h"
#include "string.h"

void printIntArr(const int *x, const int size) {
  for (int i = 0; i < size; i++) {
    printf("[%d]\n", x[i]); 
  }
}


void printSubIntArr(const int *x, const int start, const int end) {
  for (int i = start; i <= end; i++) {
    printf("[%d]\n", x[i]); 
  }
}


// int comp(const void *x, const void *y) {
//   return *(int *)x - *(int *)y; // cast to int pointer, then cast to int
// }


int abs(int x) {
  return x < 0 ? -x : x;
}


void quicksort(int *x, const int n) {
  if (n <= 1) return;

  int p_i = n - 1;
  int p_v = x[p_i];

  int copy[n];
  memcpy(copy, x, n * sizeof(int));

  int bottom = 0, top = n - 1;
  for (int i = 0; i < n; i++) {
    int v = copy[i];

    if (v >= p_v) x[top--] = v;
    else x[bottom++] = v;
  }
  
  p_i = bottom;
  x[p_i] = p_v;
  
  quicksort(x, p_i); // not p_i + 1, because p_i is sorted
  quicksort(x + p_i + 1, n - p_i - 1); // p_i + 1 because p_i is sorted
}

