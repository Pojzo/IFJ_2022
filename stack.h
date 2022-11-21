#ifndef STACK_H
#define STACK_H
#include "tokenizer.h"

typedef struct Stack {
    int top;
    int capacity;
    int *array;
} stack_t;

typedef struct Stack_Node {
    token_t *token;
} stack_node_t;

stack_t *stack_init(int capacity);
stack_node_t *stack_node_init(int data);

void stack_free(stack_t *stack);
void stack_node_free(stack_node_t *node);

#endif