#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tokenizer.h"

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
    STOP,     // '<'
    NONTERM,  // E
} symbol_enum;


bool rule_expr(token_storage_t *token_storage, bool if_while);
int convert_operator(token_t *token);
int convert_symbol(symbol_enum symbol);

#endif