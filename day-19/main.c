#include "shared.h"
#include <ctype.h>
#include <stdio.h>


typedef struct {
  StringVector stripes;
  StringVector patterns;
} StringVectorPair;


StringVectorPair initStringVectorPair() {
  StringVectorPair V;
  V.stripes  = initStringVector(INITIAL_CAPACITY);
  V.patterns = initStringVector(INITIAL_CAPACITY);

  return V;
}


void freeStringVectorPair(StringVectorPair *V) {
  freeStringVector(&V->stripes);
  freeStringVector(&V->patterns);
}


StringVectorPair readfile(const char *path) {
  StringVectorPair V = initStringVectorPair();

  FILE *file = fopen(path, "r");

  int onString = 0, stripes = 1; // 0 = patterns
  char c;
  String S;

  while ((c = getc(file)) != EOF) {
    if (isalpha(c)) {
      if (!onString) {
        S = initString(INITIAL_CAPACITY);
        onString = 1;
      }
      addChar(&S, c);

    } else if (onString) {
      if (stripes) appendString(&V.stripes, &S);
      else         appendString(&V.patterns, &S);

      if (c == '\n' && stripes) stripes = 0;
      onString = 0;
    }
  }

  fclose(file);

  return V;
}


int startsWith(const String *S, const String *P, int offset) {
  if (P->length + offset > S->length) return 0;

  for (int i = 0; i < P->length; i++)
    if (S->x[i + offset] != P->x[i]) return 0;

  return 1;
}


void printStringOffset(const String *S, int offset) {
  for (int i = offset; i < S->length; i++) printf("%c", S->x[i]);
  printf(" ");
}


int matchesPattern(const String *S, StringVector *Strings, int offset,
    int ignore) {
  if (offset >= S->length) return 1;
  
  int match = 0;
  for (int i = 0; i < Strings->length; i++) {
    if (i == ignore) continue;
    String P = Strings->x[i];
    if (P.length + offset > S->length) continue;

    if (startsWith(S, &P, offset)) {
      match = matchesPattern(S, Strings, offset + P.length, ignore);
          
    } else match = 0;


    if (match) return 1;
  }

  return 0;
}


long long int nmatchesPattern(const String *S, StringVector *Strings, int offset,
    int ignore) {
  if (offset >= S->length) return 1;
  
  long long int match = 0;
  for (int i = 0; i < Strings->length; i++) {
    if (i == ignore) continue;
    String P = Strings->x[i];
    if (P.length + offset > S->length) continue;

    if (startsWith(S, &P, offset)) {
      match += nmatchesPattern(S, Strings, offset + P.length, ignore);
    } 
  }

  return match;
}


// compare string length
int compare(const void *a, const void *b) {
  return -((String *)a)->length + ((String *)b)->length;
}


int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  StringVectorPair V = readfile(argv[1]);

  StringVector uniqueStripes = initStringVector(INITIAL_CAPACITY);
  for (int i = 0; i < V.stripes.length; i++) {
    String stripe = copyString(&V.stripes.x[i]);
    if (!matchesPattern(&stripe, &V.stripes, 0, i))
      appendString(&uniqueStripes, &stripe);
    else freeString(&stripe);
  }

  qsort(uniqueStripes.x, uniqueStripes.length, sizeof(String), compare);

  // Task 1
  int sum_t1 = 0;
  for (int i = 0; i < V.patterns.length; i++) {
    sum_t1 += matchesPattern(&V.patterns.x[i], &uniqueStripes, 0, -1);
  }
 
  printf("Answer Task 1:\n  Sum = %d\n", sum_t1);

  // Task 2
  // long long int sum_t2 = 0;
  // for (int i = 0; i < V.patterns.length; i++) {
  //   sum_t2 += matchesPattern(&V.patterns.x[i], &uniqueStripes, 0, -1);
  // }
  // printf("Answer Task 2:\n  Sum = %lld\n", sum_t2);

  freeStringVector(&uniqueStripes);
  freeStringVectorPair(&V);
 
  return 0;
}
