#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "tokenizer.h"
#include "list.h"

typedef enum {
    L_A, // left association >
    R_A, // right association <
    EQ_A, // equal association = 
    ERR,  // error
    END   // end of input
} assoc_t;

#define N 8

const int prec_table[N][N] =
{
//	  |*/ | +-.  | <>  | === | (    | )   | i  | $ |
	{ L_A , L_A , L_A , L_A , R_A , L_A , R_A , L_A }, /// */
	{ R_A , L_A , L_A , L_A , R_A , L_A , R_A , L_A }, /// +-.
	{ R_A , R_A , L_A , L_A , R_A ,  ERR  , R_A , L_A }, /// <>
	{ R_A , R_A , R_A , L_A , R_A ,  ERR  , R_A , L_A }, /// ===
	{ R_A , R_A , R_A , R_A , R_A , EQ_A  , R_A ,  ERR  }, /// (
	{ L_A , L_A , L_A , L_A ,  ERR  , L_A ,  ERR  , L_A }, /// )
	{ L_A , L_A , L_A , L_A ,  ERR  , L_A ,  ERR  , L_A }, /// i (id, int, double, string)
	{ R_A , R_A , R_A ,  ERR  , R_A ,  ERR  , EQ_A , END }  /// $
};



bool rule_expr(token_storage_t *token_storage, bool if_while) {
    (void) if_while;
    int left_brackets = 0;
    list_t *list = list_init();
    list_insert_first(&list, DOLLAR);
    symbol_enum input;
    symbol_enum top;
    int row, column;
    prec_table prec_operator; 
    bool input_loaded = false;
    while (true) { //TODO 
        if (!input_loaded) {
            token_t *token = get_token_keep(token_storage);
            if (strcmp(token->value, "(") == 0) {
                left_brackets++;
            }

            else if (strcmp(token->value, ")") == 0) {
                left_brackets--;
                if (left_brackets < 0) {
                    input_loaded = true;
                }
            }
            //MAKE SYMBOL OUT OF TOKEN 
            top = list_get_first_term(list);
            row = convert_symbol(top);
            column = convert_operator(token);
        }
        else {
            input = DOLLAR;
            top = list_get_first_term(list);
            row = convert_symbol(top);
        }
        // tuto dame ten oprator ktory budeme musiet pozuit, takze bud <, >, =, x, alebo koniec
        prec_operator = prec_table[row][column];
        // na zaklade toho aky je to operator tak rozhodneme co budeme robit 
        if (prec_operator == L_A) {
            // TODO >
        }
        else if (prec_operator == R_A) {
            // TODO < 
        }
        else if (prec_operator == EQ_A) {
            // TODO =
        }
        else if (prec_operator == ERR) {
            // TODO err
        }
        else {
            // TODO END
        }
    }
    (void) input;
    (void) row;
    (void) column;
} 

int convert_symbol(symbol_enum symbol) {
    switch(symbol) {
        case MUL:
        case DIV:
            return 0;
        case ADD:
        case SUB:
        case CONC:
            return 1;
        case GT:
        case LT:
        case GTE:
        case LTE:
            return 2;
        case NEQ:
        case EQ:
            return 3;
        case OPENBR:
            return 4;
        case CLOSEDBR:
            return 5;
        case INT:
        case FLOAT:
        case STRING:
            return 6;
        case DOLLAR:
            return 7;
        case STOP:
            return -1;
        case NONTERM:
            return -1;
    }
    return -1;
}

// -1 je error (je ne?)
int convert_operator(token_t *token) {

    if (token == NULL) {
        return 7;
    }

    if (((token->token_type == TOK_ID) && (token->value[0] == '$')) || token->token_type == TOK_LIT) {
        return 6;
    }

    if(token->token_type == TOK_ID) {
        // toto je funkcia
        // takze s tym nieco este budeme musiet spravit
        return 10;
    }

    int length = strlen(token->value);
    if (length == 1) {
        switch (token->value[0]) {
        case '*':
        case '/':
            return 0;
        case '+':
        case '-':
        case '.':
            return 1;
        case '<':
        case '>':
            return 2;
        
        case '(':
            return 4;
        case ')':
            return 5;
        }
    }
    else {
        if (strcmp(token->value, "<=") == 0 || strcmp(token->value, ">=") == 0) {
            return 2;
        }
        if (strcmp(token->value, "===") == 0 || strcmp(token->value, "!==") == 0) {
            return 3;
        }
    }
    return -1;
}