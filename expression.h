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
    STOP,     // '<' 17
    NONTERM,  // E
} symbol_enum;



bool rule_expr(token_storage_t *token_storage);
symbol_enum convert_token_to_symbol(token_t *token, bool* valid);
int convert_symbol_to_int(symbol_enum symbol);
int rule_check(symbol_enum* symbol1, symbol_enum* symbol2, symbol_enum* symbol3, int num);


#endif
