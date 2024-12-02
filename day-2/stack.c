#include "shared.h"


Stack initStack() {
  Stack stack;
  stack.x = (int*)malloc(sizeof(int)*INITIAL_STACK_SIZE);
  stack.size = 0;
  stack.capacity = INITIAL_STACK_SIZE;

  return stack;
}


int pop(Stack *stack) {
  if (stack->size < 1) {
    printf("Attempted to pop element of empty stack!\n");
    return -1;
  }

  stack->size--;
  return stack->x[stack->size];
}


int top(Stack *stack) {
  if (stack->size < 1) {
    printf("Attempted to top element of empty stack!\n");
    return -1;
  }

  return stack->x[stack->size-1];
}


void put(Stack *stack, int value) {
  if (stack->size >= stack->capacity) {
    stack->capacity = stack->capacity * 2;
    stack->x = realloc(stack->x, stack->capacity * sizeof(int)); 
  }

  stack->x[stack->size] = value;
  stack->size++;
}


void freeStack(Stack *stack) {
  free(stack->x);
}


void resetStack(Stack *stack) {
  stack->size = 0;
}
