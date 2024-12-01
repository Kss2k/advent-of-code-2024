#ifndef shared_lib

#define shared_lib

// utils
void printIntArr(const int *x, const int size);
void printSubIntArr(const int *x, const int start, const int end);


// read file/array
typedef struct {
  int *x;
  int *y;
  int length_x;
  int length_y;
} TwoArrays;


TwoArrays readFileAllocateArrays(const char *path);
void deleteArrays(TwoArrays arrays);


#endif // !shared_lib 


