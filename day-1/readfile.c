#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"


TwoArrays readFileAllocateArrays(const char *path) {
  FILE *fileptr = fopen(path, "r");

  TwoArrays arrays;
  printf("file = %s\n", path);

  if (fileptr == NULL) {
    printf("could not open file!\n");
    return arrays;
  }

  fseek(fileptr, 0L, SEEK_END);
  long int size = ftell(fileptr);

  printf("size = %ld\n", size);
  int *x = (int*)malloc(size);
  int *y = (int*)malloc(size);
  
  char c;
  int is_x = 1;
  int num = 0;
  int pos_y = 0;
  int pos_x = 0;
  int on_number = 0;

  fseek(fileptr, 0L, SEEK_SET);
  while ((c = fgetc(fileptr)) != EOF) {
    if ((c == '\n' || c == ' ') && on_number) {
      if (is_x) {
        x[pos_x++] = num;
        is_x = 0;
      } else {
        y[pos_y++] = num;
        is_x = 1;
      }
      num = 0;
      on_number = 0;

    } else if (isdigit(c)) {
      on_number = 1;
      num = num * 10 + (int)(c - '0');
    }
  }

  fclose(fileptr); // dont forget!
  
  arrays.x = x; arrays.y = y;
  arrays.length_x = pos_x;
  arrays.length_y = pos_y;

  return arrays;
}


void freeArrays(TwoArrays arrays) {
  free(arrays.x);
  free(arrays.y);
}
