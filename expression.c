#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "stack.h"
#include "tokenizer.h"

typedef enum {
    L_A, // left association >
    R_A, // right association 
    E_A, // equal association
    E    // error
} assoc_t;

const int N = 8;

int prec_table[N][N] =
{
//	  |*/ | +-.  | <>  | === | (    | )   | i  | $ |
	{ L_A , L_A , L_A , L_A , R_A , L_A , R_A , L_A }, /// */
	{ R_A , L_A , L_A , L_A , R_A , L_A , R_A , L_A }, /// +-.
	{ R_A , R_A , L_A , L_A , R_A ,  X  , R_A , L_A }, /// <>
	{ R_A , R_A , R_A , L_A , R_A ,  X  , R_A , L_A }, /// ===
	{ R_A , R_A , R_A , R_A , R_A , EQ  , R_A ,  X  }, /// (
	{ L_A , L_A , L_A , L_A ,  X  , L_A ,  X  , L_A }, /// )
	{ L_A , L_A , L_A , L_A ,  X  , L_A ,  X  , L_A }, /// i (id, int, double, string)
	{ R_A , R_A , R_A ,  X  , R_A ,  X  , E_A , KONIEC }  /// $
};


bool rule_expr(token_storage_t *token_storage) {
    get_token(token_storage);
        return 1;
}


bool rule_expr(token_storage_t *token_storage) {
    
}

int convert_operator(token_t *token) {
    switch(token->value[0]) {
        case '+':
        case '-':
            return 0;
        case '*':
        case 
    }
}