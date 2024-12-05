#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  int *x;
  long int length;
  long int capacity;
} Array;


typedef struct {
  Array *x;
  long int length;
  long int capacity;
} ArrayArray;


typedef struct {
  Array before;
  Array after;
  ArrayArray orders;
} Arrays;


ArrayArray initArrayArray(int capacity) {
  ArrayArray X;
  X.x = (Array*)malloc(capacity * sizeof(Array));
  X.length = 0;
  return X;
}


Array initArray(int capacity) {
  Array X;
  X.x = (int*)malloc(capacity * sizeof(int));
  X.capacity = capacity;
  X.length = 0;
  return X;
}


void AddArray(ArrayArray *X) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (Array*)realloc(X->x, X->capacity * sizeof(Array));
  }
  X->x[X->length++] = initArray(10);
}


void append(Array *X, int x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (int*)realloc(X->x, X->capacity * sizeof(int));
  }
  X->x[X->length++] = x;
}


void freeArray(Array *X) {
  free(X->x);
}


void freeArrayArray(ArrayArray *X) {
  for (int i = 0; i < X->length; i++) {
    freeArray(&X->x[i]);
  }
  free(X->x);
}


void printArrayNoNewline(Array X) {
  printf("[");
  for (int i = 0; i < X.length; i++) {
    printf("%d", X.x[i]);
    if (i + 1 < X.length) printf(", ");
  }
  printf("]");
}


void printArray(Array X) {
  printArrayNoNewline(X);
  printf("\n");
}


void printArrayArray(ArrayArray X) {
  printf("[");
  for (int i = 0; i < X.length; i++) {
    if (i) printf(" ");
    printArrayNoNewline(X.x[i]);
    if (i + 1 < X.length) printf(",\n");
  }
  printf("]\n");
}


Arrays readFile(const char *path) {
  FILE *file = fopen(path, "r");
  Arrays X;

  if (file == NULL) {
    printf("Could not open file!\n");
    return X;
  }

  fseek(file, 0L, SEEK_END); 
  long int length = ftell(file) / sizeof(char);

  Array before = initArray(length / 2);
  Array after  = initArray(length / 2);
  fseek(file, 0L, SEEK_SET);

  char c;
  int newlineCounter = 0, onBefore = 1, i = 0, num = 0, onum = 0;

  while ((c = getc(file)) != EOF && newlineCounter <= 1) {
    if (c == '\n') {
      if (onum) append(&after, num);
      onum = 0;
      num = 0;
      onBefore = 1;
      newlineCounter++;

    } else if (c == '|') {
      if (onum) append(&before, num);
      onum = 0;
      num = 0;
      onBefore = 0;
      newlineCounter = 0;

    } else if (isdigit(c)) {
      num = num * 10 + c - '0';
      newlineCounter = 0;
      onum = 1;

    } else printf("Unexpected character: %c\n", c);
  }

  ArrayArray orders = initArrayArray(10);

  num = 0;
  onum = 0;
  int onarray = 0;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      onarray = 0;      
    } else if (isdigit(c)) {
      num = num * 10 + c - '0';
      onum = 1;
    } else if (c == ',') {
      if (!onarray) {
        AddArray(&orders);
        onarray = 1;
      }
      if (onum) append(&orders.x[orders.length-1], num);
      onum = 0;
      num = 0;
    } else printf("Unexpected character: %c\n", c);
  }

  fclose(file);


  X.before = before;
  X.after = after;
  X.orders = orders;

  return X;
}


int main(int argv, char *argc[]) {
  if (argv < 2) {
    printf("Expected a file input!\n");
    return -1;
  }
  Arrays X = readFile(argc[1]);
  
  printArray(X.before);
  printArray(X.after);
  printArrayArray(X.orders);

  freeArray(&X.before);
  freeArray(&X.after);
  freeArrayArray(&X.orders);

  return 0;
}
