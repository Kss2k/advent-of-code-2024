#include <stdio.h>


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
