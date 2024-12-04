#include <stdio.h>
#include <stdlib.h>


typedef struct {
  char *x;
  long int length;
} Array;


Array readFile(const char *path) {
  FILE *file = fopen(path, "r");
  Array Y;

  if (file == NULL) {
    printf("Could not open file!\n");
    return Y;
  }

  fseek(file, 0L, SEEK_END); 
  long int size = ftell(file);

  char *X = (char*)malloc(size);
  fseek(file, 0L, SEEK_SET);

  char c;
  int i;
  for (i = 0; (c = getc(file)) != EOF && i < size / sizeof(char); i++) {
    X[i] = c; 
  }
  X[i] = '\0';

  fclose(file);

  Y.x = X;
  Y.length = size / sizeof(char);

  return Y;
}


int startsWith(const char *x, const char *p) {
  #define MAX_LOOP_SW 1000
  for (int i = 0; p[i] != '\0' && i < MAX_LOOP_SW; i++) {
    if (p[i] != x[i]) return 0;
  }

  return 1;
}


int startsWithXOrY(const char *x1, const char *x2, const char *p) {
  return startsWith(x1, p) || startsWith(x2, p);
}


int getWidth(const char *x) {
  #define MAX_LOOP_GW 1000
  int i;
  for (i = 0; x[i] != '\0' && x[i] != '\n' && i < MAX_LOOP_GW; i++);
  return i + 1;
}


int main(int argv, char *argc[]) {
  if (argv < 2) {
    printf("Expected a file input!\n");
    return -1;
  }
  Array Y = readFile(argc[1]);
  char *X = Y.x;
  
  const char XMAS[] = "XMAS";
  const char SAMX[] = "SAMX";
  const int wlen = 4;
  char currentWord[wlen + 1]; // remember '\0'
  int currentWlen = 0;

  const int width = getWidth(X);
  const int length = Y.length / width;

  int sum_t1 = 0;

  #define NEW_CHARACTER do { \
    currentWord[currentWlen++] = c; \
    currentWord[currentWlen] = '\0'; \
  } while(0) \

  #define SOFT_RESET do { \
    currentWlen = 0; \
    currentWord[currentWlen++] = c; \
    currentWord[currentWlen] = '\0'; \
  } while(0) \

  #define HARD_RESET do { \
    currentWlen = 0; \
    currentWord[0] = '\0'; \
  } while(0) \

  #define RESET_NO_MATCH do { \
    if (!startsWithXOrY(XMAS, SAMX, currentWord)) { \
      SOFT_RESET; \
      continue; \
    } \
  } while(0) \

  #define CHECK_IF_FULL_MATCH(sum) do { \
    if (currentWlen >= wlen) { \
      SOFT_RESET; \
      sum++; \
    } \
  } while(0) \
  
  #define MEAT(sum) do { \
    NEW_CHARACTER; \
    RESET_NO_MATCH; \
    CHECK_IF_FULL_MATCH(sum); \
  } while(0) \

  // Check forward and backwards
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      char c = X[i * width + j];
      MEAT(sum_t1);
    }
  
    HARD_RESET;
  }
 
  // Check up and down
  for (int j = 0; j < width; j++) {
    for (int i = 0; i < length; i++) {
      char c = X[i * width + j];
      MEAT(sum_t1);
    }
  
    HARD_RESET;
  }

  // Check (left-right) diagonally (forwards and backwards)
  for (int i = 0; i < length; i++) {
    int searchWidth = !i ? width : 1;
    for (int j = 0; j < searchWidth; j++) {
      
      for (int k = 0; k + j < width && k + i < length; k++) {
        char c = X[(i + k) * width + j + k];
        MEAT(sum_t1);
      }

      HARD_RESET;
    }
  }
  
  // Check (left-right) diagonally (forwards and backwards)
  for (int i = 0; i < length; i++) {
    int searchWidth = !i ? width : 1;
    for (int j = width - 2; j >= width - searchWidth - 1 && j > 0; j--) {
  
      for (int k = 0; j - k >= 0 && k + i < length; k++) {
        char c = X[(i + k) * width + j - k];
        MEAT(sum_t1);
      }

      HARD_RESET;
    }
  }
  printf("Answer Task 1:\n  Sum = %d\n", sum_t1);

  free(X);

  return 0;
}
