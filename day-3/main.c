#include <ctype.h>
#include <stdio.h>


int parseMul(FILE *file) {
  fflush(file);
  unsigned long origPos = ftell(file);
  int match = getc(file) == 'u' && getc(file) == 'l'; // check before l in case of EOF

  if (!match) fseek(file, origPos, SEEK_SET);
  return match;
}


int parseDoOrDont(FILE *file, int current) {
  fflush(file);
  unsigned long origPos = ftell(file);
  int isDo = getc(file) == 'o' && getc(file) == '('
    && getc(file) == ')';
  if (!isDo) fseek(file, origPos, SEEK_SET);
  else return 1;

  int isDont = getc(file) == 'o' && getc(file) == 'n' && 
    getc(file) == '\'' && getc(file) == 't' && getc(file) == '('
    && getc(file) == ')';

  if (!isDont) fseek(file, origPos, SEEK_SET);
  else return 0;

  return current;
}


int parseClosure(FILE *file) {
  #define RETURN_AND_RESET do {fseek(file, origPos, SEEK_SET); return 0;} while(0)

  fflush(file);
  unsigned long origPos = ftell(file);
  char c;
  int nnum = 0, ndigit = 0, onlhs = 1, lhs = 0, rhs = 0, onnum = 0;

  while ((c = getc(file)) != EOF) {
    if (!onnum && !isdigit(c)) RETURN_AND_RESET;
    else if (onnum && c == ',') {
      if (nnum != 1) RETURN_AND_RESET;
      onlhs = 0;
      onnum = 0;
      ndigit = 0;

    } else if (isdigit(c)) {
      if (++ndigit > 3) RETURN_AND_RESET;
      if (onlhs) lhs = lhs * 10 + (int)(c - '0');
      else       rhs = rhs * 10 + (int)(c - '0');
      if (!onnum) {nnum++; onnum = 1;}

    } else if (c == ')') {
      if (nnum != 2) RETURN_AND_RESET;
      return lhs * rhs;
    } else RETURN_AND_RESET;
    
  }

  RETURN_AND_RESET;
}


int main(int argv, char *argc[]) {
  if (argv < 2) {
    printf("Expected a file input!\n");
    return -1;
  }

  FILE *file = fopen(argc[1], "r");

  if (file == NULL) {
    printf("Could not open file!\n");
    return -1;
  }

  fseek(file, 0L, SEEK_SET); 

  int mulFound = 0;
  char c;
  int sum_t1 = 0;
  while ((c = getc(file)) != EOF) {
    switch (c) {
      case 'm': {
        mulFound = parseMul(file);
        break;
      }
      case '(': {
        sum_t1 += mulFound ? parseClosure(file) : 0;
        mulFound = 0;
        break;
      }
      default: {
        mulFound = 0;
        break;
      }
    }

    if (feof(file)) break;
  }

  printf("Answer task 1:\n  Sum = %d\n", sum_t1);
  
  // Task 2
  fseek(file, 0L, SEEK_SET);
  mulFound = 0;
  int sum_t2 = 0;
  int eval = 1;
  while ((c = getc(file)) != EOF) {
    switch (c) {
      case 'm': {
        mulFound = parseMul(file);
        break;
      }
      case 'd': {
        eval = parseDoOrDont(file, eval);
        break;
      }
      case '(': {
        sum_t2 += mulFound && eval ? parseClosure(file) : 0;
        mulFound = 0;
        break;
      }
      default: {
        mulFound = 0;
        break;
      }
    }

    if (feof(file)) break;
  }

  printf("Answer task 1:\n  Sum = %d\n", sum_t2);

  fclose(file);

  return 0;
}
