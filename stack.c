#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

stack_t *stack_init(int capacity) {
    stack_t *stack = malloc(sizeof(stack_t));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = malloc(sizeof(int) * capacity);
    return stack;
}

stack_node_init(token_t *token) {
    stack_node_t *node = malloc(sizeof(stack_node_t));
    node->token = token;
    return node;
}

void stack_free(stack_t *stack) {
    free(stack->array);
    free(stack);
}

void stack_node_free(stack_node_t *node) {
    free(node);
}