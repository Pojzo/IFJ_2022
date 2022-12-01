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

int prec_table[N][N] =
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
    (void) token_storage;
    (void) if_while;
    return true;
} 

int convert_operator(token_t *token) {
    if (((token->token_type == TOK_ID) && (token->value[0] == '$')) || token->token_type == TOK_LIT) {
        return 6;
    }

    if(token->token_type == TOK_ID) {
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
    return 7;
}