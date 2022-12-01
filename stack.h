#ifndef STACK_H
#define STACK_H
#include "tokenizer.h"

typedef struct Stack {
    int top;
    int capacity;
    token_t *array;
} stack_t;

stack_t *stack_init(int capacity);
void stack_free(stack_t *stack);

void stack_push(stack_t *stack, token_t *token);
token_t *stack_pop(stack_t *stack);

#endif