#ifndef SHARED_H

// headers
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


// stack.c
#define INITIAL_STACK_SIZE 4
typedef struct {
  int *x;
  int size;
  int capacity;
} Stack;

Stack initStack();
int pop(Stack *stack);
int top(Stack *stack);
void put(Stack *stack, int value);
void freeStack(Stack *stack);
void resetStack(Stack *stack);


// utils.c
int getSign(int x);
int isValidDiff(int x0, int x1, int *sign);
int canBeValid(Stack *stack);


#endif
