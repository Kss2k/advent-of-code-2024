#include "shared.h"


Arrays readfile(const char *path) {
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

  ArrayArray orders = initArrayArray(INITIAL_CAPACITY);

  num = 0;
  onum = 0;
  int onarray = 0;
  while ((c = getc(file)) != EOF) {
    if (c == ',' || c == '\n') {
      if (!onarray) {
        AddArray(&orders);
        onarray = 1;
      }
      if (onum) append(&orders.x[orders.length-1], num);
      onum = 0;
      num = 0;

      if (c == '\n') onarray = 0;

    } else if (isdigit(c)) {
      num = num * 10 + c - '0';
      onum = 1;
    } else printf("Unexpected character: %c\n", c);
  }

  fclose(file);

  X.before = before;
  X.after = after;
  X.orders = orders;

  return X;
}
