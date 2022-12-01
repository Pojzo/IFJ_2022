#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "stack.h"
#include "tokenizer.h"

typedef enum {
    L_A, // left association >
    R_A, // right association 
    EQ_A, // equal association
    ERR,
    END
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



int left_brackets = 0;
bool rule_expr(token_storage_t *token_storage, bool if_while) {
    bool input_end = false;
    left_brackets = 0;
    char *input;
    char *stack_top;
    while (true) {
        if (input_end == 0) {
            token_t *cur_token = get_token_keep();
            if (cur_token->token_type == TOK_OPERATOR && strcmp(cur_token->value, "(") == 0) {
                left_brackets++;
            }

            else if(cur_token->token_type == TOK_OPERATOR && strcmp(cur_token->value, ")") == 0) {
                if (left_brackets == 0) {
                    input_end = 1;
                    continue;
                }
            }

            input = "(";
        }

        else {
            input = '$';
        }
        
    }

    while (true)  { // stack != $S

    }

    return true;
} 

// -1 je error (je ne?)
int convert_operator(token_t *token, bool if_while) {
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