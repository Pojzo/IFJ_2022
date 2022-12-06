#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef enum {
    INT,      // 5
    FLOAT,    // 5.5
    STRING,   // "bratislava"
    ADD,     // + 
    SUB,    // - 
    MUL,      // *
    DIV,      // /
    CONC,     // .
    NEQ,      // !==
    EQ,       // === 
    GT,       // >
    LT,       // < 
    GTE,      // >=  
    LTE,      // <=
    DOLLAR,   // 
    OPENBR,   // (
    CLOSEDBR, // )
    STOP,     // '<' 17
    NONTERM,  // E
} symbol_enum;

/*
typedef struct Element {
    symbol_enum symbol;
    token_t* token;
} element_t;
*/

typedef struct List {
    symbol_enum symbol;
    struct List* next;
} list_t;

list_t *list_init();
void print_list(list_t *list);
void list_free(list_t *list);
void list_insert_first(list_t **list, symbol_enum symbol);
void list_insert_after_nonterm(list_t **list);
symbol_enum list_pop_first(list_t **list);
symbol_enum list_get_first_term(list_t *list);
bool final_condition(list_t* list);
int return_before_stop(list_t** list, symbol_enum* symbol1, symbol_enum* symbol2, symbol_enum* symbol3, int* num); 
int symbols_till_stop(list_t* list);

#endif
