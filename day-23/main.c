#include "shared.h"
#include <stdio.h>

#define NCHARS 27

typedef struct {
  long int items[NCHARS * NCHARS];
} TwoCharHashTable;

TwoCharHashTable HashTable;


long int hash(String *key) {
  if (key->length < 2) {
    printf("String to short to be hashed!\n");
    exit(-1);
  }
  return key->x[0] - 'a' + NCHARS * (key->x[1] - 'a');
}


typedef struct {
  Matrix M;
  StringVector labels;
} SMatrix;


void printSMatrix(SMatrix *M) {
  printf("    ");
  for (int j = 0; j < M->labels.length; j++) {
    printString(&M->labels.x[j]); printf(" ");
  }
  printf("\n");
  for (int i = 0; i < M->M.nrow; i++) {
    printString(&M->labels.x[i]); printf(" ");
    for (int j = 0; j < M->M.ncol; j++) {
      if (M->M.x[i][j]) printf("  %d", M->M.x[i][j]);
      else printf("  .");
    }
    printf("\n");
  }
}

void freeSMatrix(SMatrix *M) {
  freeMatrix(&M->M);
  freeStringVector(&M->labels);
}


SMatrix readfile(const char *path) {
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    printf("Unable to open file: %s!\n", path);
    exit(-1);
  }

  char c, mode = 'n';
  String S;
  
  StringVector lhs = initStringVector(INITIAL_CAPACITY), 
               rhs = initStringVector(INITIAL_CAPACITY);

  while((c = getc(file)) != EOF) {
    if (isalpha(c)) {
      S = initString(2);
      addChar(&S, c); 
      addChar(&S, getc(file));
      mode = mode == 'n' ? 'l': 'r';

    } else if (c == '\n') {
      mode = 'n'; 
      continue;
    } else continue;

    if      (mode == 'l') appendString(&lhs, &S);
    else if (mode == 'r') appendString(&rhs, &S);
  }
  fclose(file);
 
  if (lhs.length != rhs.length) {
    printf("Unequal lengths on rhs and lhs!\n");
    exit(-1);
  }

  StringVector labels = initStringVector(INITIAL_CAPACITY);

  int k = 0;
  for (int i = 0; i < lhs.length; i++) {
    if (!isinStringVector(lhs.x[i], &labels)) {
      S = copyString(&lhs.x[i]);
      appendString(&labels, &S);
      HashTable.items[hash(&S)] = k++;       
    }
    if (!isinStringVector(rhs.x[i], &labels)) {
      S = copyString(&rhs.x[i]);
      appendString(&labels, &S);
      HashTable.items[hash(&S)] = k++;
    }
  }


  SMatrix M;
  M.M = initMatrix(labels.length, labels.length);
  M.labels = labels;
  fillMatrix(&M.M, 0);

  for (int i = 0; i < lhs.length; i++) {
    long int lkey = HashTable.items[hash(&lhs.x[i])], 
         rkey = HashTable.items[hash(&rhs.x[i])];
    M.M.x[lkey][rkey] = 1, M.M.x[rkey][lkey] = 1;
    M.M.x[lkey][lkey] = 1, M.M.x[rkey][rkey] = 1;
  }

  freeStringVector(&lhs);
  freeStringVector(&rhs);

  return M;
}


int allSMatrix(SMatrix *M, Vector *ignore) {
  for (int i = 0; i < M->M.nrow; i++) {
    if (isin(i, ignore)) continue;

    for (int j = 0; j < M->M.ncol; j++) {
      if (isin(j, ignore)) continue;

      if (!M->M.x[i][j]) return 0;
    }
  }

  return 1;
}


StringVector largestLAN(SMatrix *M, Vector *ignore, StringVector *lan, int offset) {
  if (ignore->length == M->M.nrow) return copyStringVector(lan);
  else if (allSMatrix(M, ignore)) {
    StringVector out = copyStringVector(lan);

    for (int i = offset; i < M->labels.length; i++) {
      if (isin(i, ignore)) continue;
      String computer = copyString(&M->labels.x[i]);
      appendString(&out, &computer);
    }

    return out;
  }

  StringVector out = copyStringVector(lan); 

  for (int i = offset; i < M->M.nrow; i++) {
    if (isin(i, ignore)) continue;

    String computer = copyString(&M->labels.x[i]);
    Vector ignoreNext = copyVector(ignore);
    StringVector lanNext = copyStringVector(lan);

    appendString(&lanNext, &computer);
    append(&ignoreNext, i);

    for (int j = offset + 1; j < M->M.nrow; j++)
      if (!M->M.x[i][j] && !isin(j, &ignoreNext)) append(&ignoreNext, j);
    
    StringVector result = largestLAN(M, &ignoreNext, &lanNext, offset + 1);
    freeVector(&ignoreNext);
    freeStringVector(&lanNext);

    if (result.length > out.length) {
      freeStringVector(&out);
      out = result;

      if (result.length >= M->M.ncol - ignore->length - 1)
        return out;

    } else freeStringVector(&result);
  }

  return out;
}


int compareString(const void *x, const void *y) {
  String s0 = *(String*)x, s1 = *(String*)y;
  for (int i = 0; i < s0.length && i < s1.length; i++) {
    if (s0.x[i] > s1.x[i])      return 1;
    else if (s0.x[i] < s1.x[i]) return -1;
  }

  return s0.length - s1.length;
}


void printLanString(StringVector *X) {
  for (int i = 0; i < X->length; i++) {
    printString(&X->x[i]);
    if (i + 1 < X->length) printf(",");
  }
  printf("\n");
}



int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Expected file arg!\n");
    return -1;
  } 

  SMatrix M = readfile(argv[1]);

  int sum_t1 = 0;

  for (int i = 0; i < M.M.nrow; i++) {
    for (int j = i + 1; j < M.M.nrow; j++) {
      if (M.M.x[i][j] != 1) continue;

      for (int k = j + 1; k < M.M.nrow; k++) {
        if (M.M.x[i][k] != 1 || M.M.x[j][k] != 1) continue;
        else if (M.labels.x[i].x[0] != 't' && 
            M.labels.x[j].x[0] != 't' &&
            M.labels.x[k].x[0] != 't') continue;

        sum_t1++;
      } 
    }
  }
  printf("Answer Task 1:\n  Sum = %d\n", sum_t1);
  
  Vector ignore = initVector(INITIAL_CAPACITY);
  StringVector lan = initStringVector(INITIAL_CAPACITY);
  StringVector result = largestLAN(&M, &ignore, &lan, 0);
  qsort(result.x, result.length, sizeof(String), compareString);
  printLanString(&result);
 
  freeStringVector(&lan);
  freeStringVector(&result);
  freeVector(&ignore);
  freeSMatrix(&M);
  return 0;
}
