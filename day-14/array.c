#include "shared.h"
#include "limits.h"

Vector initVector(int capacity) {
  Vector X;
  X.x = (ARR_TYPE *)malloc(capacity * sizeof(ARR_TYPE));
  X.capacity = capacity;
  X.length = 0;
  return X;
}

Vector copyVector(Vector *X) {
  Vector Y;
  Y.x = (ARR_TYPE *)malloc(X->capacity * sizeof(ARR_TYPE));
  Y.length = X->length;
  Y.capacity = X->capacity;
  memcpy(Y.x, X->x, X->length * sizeof(ARR_TYPE));
  return Y;
}


void append(Vector *X, ARR_TYPE x) {
  if (X->length == X->capacity) {
    X->capacity *= 2;
    X->x = (ARR_TYPE *)realloc(X->x, X->capacity * sizeof(ARR_TYPE));
  }
  X->x[X->length++] = x;
}


void concat(Vector *X, Vector *Y) {
  for (int i = 0; i < Y->length; i++) {
    append(X, Y->x[i]);
  }
}


void resetVector(Vector *X) {
  X->length = 0;
}

void freeVector(Vector *X) { 
  free(X->x); 
}


// void printVectorNoNewline(Vector *X) {
//   printf("[");
//   for (int i = 0; i < X->length; i++) {
//     printf("%ld", X->x[i]);
//     if (i + 1 < X->length)
//       printf(", ");
//   }
//   printf("]");
// }
// 
// 
// void printVector(Vector *X) {
//   printVectorNoNewline(X);
//   printf("\n");
// }
// 
// 
// int which(Vector *X, ARR_TYPE x) {
//   for (int i = 0; i < X->length; i++)
//     if (X->x[i] == x)
//       return (i);
//   return -1;
// }
// 
// 
// int isin(ARR_TYPE x, Vector *X) { return which(X, x) != -1; }
// 
// 
// int which2D(Vector *X, Vector *Y, ARR_TYPE x, ARR_TYPE y) {
//   if (X->length != Y->length) {
//     printf("which2D() called on arrays of unequal lengths!\n");
//   }
// 
//   for (int i = 0; i < X->length; i++) {
//     if (x == X->x[i] && y == Y->x[i])
//       return i;
//   }
// 
//   return -1;
// }
// 
// 
// int isin2D(ARR_TYPE x, ARR_TYPE y, Vector *X, Vector *Y) {
//   return which2D(X, Y, x, y) != -1;
// }
// 
// 
// long long firstMisMatch(Vector *X, Vector *Y) {
//   if (X->length != Y->length) return -2;
//   for (int i = 0; i < X->length; i++) if (X->x[i] != Y->x[i]) return i;
//   return -1;
// }
// 
// 
// long long lastMisMatch(Vector *X, Vector *Y) {
//   if (X->length != Y->length) return -2;
//   for (int i = X->length - 1; i >= 0; i--) if (X->x[i] != Y->x[i]) return i;
//   return -1;
// }
// 
// 
// int isequal(Vector *X, Vector *Y) {
//   if (X->length != Y->length) return 0;
//   for (int i = 0; i < X->length; i++) if (X->x[i] != Y->x[i]) return 0;
// 
//   return 1;
// }
// 
// 
// ARR_TYPE pop(Vector *X, const int i) {
//   if (i < 0 || i >= X->length) {
//     printf("Warning: Trying to pop value outside of range (%d)!\n", i);
//     printf("Vector:\n");
//     printVector(X);
//     return -1;
//   }
// 
//   ARR_TYPE x = X->x[i];
//   for (int j = i; j < X->length - 1; j++) {
//     X->x[j] = X->x[j + 1]; // in the case where i is the last element, 
//                            // we do nothing, and decrement only X->length
//   }
// 
//   X->length--;
// 
//   return x;
// }
// 
// 
// ARR_TYPE maxVector(Vector *X) {
//   ARR_TYPE max = INT_MIN;
//   
//   for (int i = 0; i < X->length; i++) 
//     max = X->x[i] > max ? X->x[i]: max;
// 
//   return max;
// }
