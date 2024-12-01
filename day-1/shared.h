#ifndef shared_lib

#define shared_lib


// utils.c ---------------------------------------------------------------------
void printIntArr(const int *x, const int size);
void printSubIntArr(const int *x, const int start, const int end);
// int comp(const void *x, const void *y); // comparison for qsort()
int abs(int x);
void quicksort(int *x, const int n);


// readfile.c ------------------------------------------------------------------
typedef struct {
  int *x;
  int *y;
  int length_x;
  int length_y;
} TwoArrays;

TwoArrays readFileAllocateArrays(const char *path);
void freeArrays(TwoArrays arrays);


#endif // !shared_lib 


