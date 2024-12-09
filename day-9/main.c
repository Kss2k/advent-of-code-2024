#include "shared.h"


void printDisk(Array *X) {
  for (int i = 0; i < X->length; i++) {
    if (X->x[i] == -1) printf(".");
    else printf("%d", X->x[i]);
  }
  printf("\n");
}


void moveR2LDiskT1(Array *X) {
  int j;
  for (int i = X->length - 1; i >= 0; i--) {
    if ((j = which(X, -1)) == -1 || j > i) return;

    X->x[j] = X->x[i];
    X->x[i] = -1;
  }
}


void moveR2LDiskT2(Array *X) {
  int j;
  int length_free, length_file, id;
  for (int i = X->length - 1; i >= 0;) {
    if ((j = which(X, -1)) == -1 || j > i) return;
   
    // if . or id is larger than previous, ignore
    if (X->x[i] == -1 || (i < X->length - 1 && X->x[i] >= id)) {
      i--;
      continue;
    }

    id = X->x[i];

    // first get length of the file
    length_file = 0;
    for (int p = i; p >= 0; p--) {
      if (X->x[p] == X->x[i]) length_file++;
      else break;
    }

    // Then get the first (if any) free space matching the size
    length_free = 0;
    for (int k = j; k < X->length && k < i; k++) {
      if (X->x[k] != -1) {
        length_free = 0;
        continue;
      }

      j = !length_free ? k: j;
      if (++length_free >= length_file) break;
    }

    // if no space, decrease i, and continue
    if (length_free < length_file) {
      i -= length_file; 
      continue;
    }

    // if space fill
    for (int k = 0; k < length_file; k++) {
      X->x[j++] = id;
      X->x[i--] = -1;
    }
  }
}


long long int calcCheckSum(Array *X) {
  long long int sum = 0;

  for (int i = 0; i < X->length; i++) {
    if (X->x[i] == -1) continue; // This can be more efficient if we assume the
                                 // DISK/array is always sorted
    sum += i * X->x[i];    
  }

  return sum;
}


Array readfile(const char *path) {
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    printf("Unable to open file!\n");
    exit(-1);
  }

  Array X = initArray(INITIAL_CAPACITY);

  int fileBlock = 1, id = 0, size, fill;
  char c;
  while ((c = getc(file)) != EOF) {
    if (!isdigit(c)) continue;

    size = c - '0';
    fill = fileBlock ? id++: -1;
    fileBlock = fileBlock ? 0: 1;

    for (int i = 0; i < size; i++) append(&X, fill);
  }

  fclose(file);
  return X;
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Missing file arg!\n");
    exit(-1);
  }

  Array X = readfile(argv[1]);
  Array Y = copyArray(&X);

  // Task 1
  moveR2LDiskT1(&X);
  long long int sum_t1 = calcCheckSum(&X);
  printf("Answer Task 1:\n  Sum = %lld\n", sum_t1);

  // Task 2
  moveR2LDiskT2(&Y);
  long long int sum_t2 = calcCheckSum(&Y);
  printf("Answer Task 2:\n  Sum = %lld\n", sum_t2);

  freeArray(&X);
  freeArray(&Y);

  return 0;
}
