#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <assert.h>

#define CAPACITY 200

stack_t *stack_init(int capacity) {
    stack_t *stack = malloc(sizeof(stack_t));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = malloc(sizeof(token_t) * CAPACITY);
    return stack;
}

void stack_free(stack_t *stack) {
    free(stack->array);
    free(stack);
}

void stack_node_free(stack) {
    free(node);
}

void stack_push(stack_t *stack, tok_type token_type, char *token_value) {
    assert(top + 1 < CAPACITY);
    token_t *token = token_create(token_type, token_value, strlen(token_value));
    stack->array[++top] = token;
}

token_t *stack_pop(stack_t *stack) {
    if (top == -1) {
        return NULL;
    }
    token_t* token = stack->array[stack->top];
    free(stack->array[stack->top--])
    return token;
}

token_t *stack_pop(stack_t *stack) {
    if (top == -1) {
        return NULL;
    }
    return stack->array[top];
}