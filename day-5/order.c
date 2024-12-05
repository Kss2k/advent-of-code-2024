#include "shared.h"


int isOrderValid(Array order, Array before, Array after) {
  if (before.length != after.length) printf("Unequal length of before and after!");
  for (int i = 0; i < before.length; i++) {
    int locb = which(order, before.x[i]);
    int loca = which(order, after.x[i]);
    if (locb == -1 || loca == -1) continue;
    if (locb > loca) return 0;
  }

  return 1;
}


int isAfterOrSame(int x, int y, Array *before, Array *after) {
  for (int i = 0; i < before->length; i++) {
    if (before->x[i] == x && after->x[i] == y) return 0;
    else if (after->x[i] == x && before->x[i] == y) return 1;
  } 

  return 1;
}


void quicksort(int *x, int n, Array *before, Array *after) {
  if (n <= 1) return;

  int p_i = n - 1;
  int p_v = x[p_i];

  int copy[n];
  memcpy(copy, x, n * sizeof(int));

  int bottom = 0, top = n - 1;
  for (int i = 0; i < n; i++) {
    int v = copy[i];

    if (isAfterOrSame(v, p_v, before, after)) x[top--] = v;
    else x[bottom++] = v;
  }
  
  p_i = bottom;
  x[p_i] = p_v;
  
  quicksort(x, p_i, before, after); // not p_i + 1, because p_i is sorted
  quicksort(x + p_i + 1, n - p_i - 1, before, after); // p_i + 1 because p_i is sorted
}


void fixOrder(Array *order, Array *before, Array *after) {
  int n = order->length;
  int *x = order->x;
  quicksort(x, n, before, after);
}
