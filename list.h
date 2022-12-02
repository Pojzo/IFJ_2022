#ifndef STACK_H
#define STACK_H

#include "expression.h"

typedef struct List {
    symbol_enum symbol;
    struct List* next;
} list_t;

list_t *list_init();
void list_free(list_t *list);
void list_insert_first(list_t **list, symbol_enum symbol);
void list_insert_after_nonterm(list_t **list);
void list_pop_first(list_t **list);
bool final_condition(list_t* list);
//symbol_enum condition(liget_first_term(list_t *list));

#endif
