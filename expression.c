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

typedef struct {
    token_t *token;
    symbol_enum symbol;
    bool is_expr; 
    //expr_t *expr;
} token_symbol_t;

typedef struct {
    //expr_t *left;
    //expr_t *right;
    token_symbol_t* token_symbol;
} expr_t;


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
	{ R_A , R_A , R_A ,  ERR  , R_A ,  ERR  , R_A , END }  /// $
};



bool rule_expr(token_storage_t *token_storage) {
    static int num_calls = 0;
    for (int i = 0; i< 50; i++) {
        // print num_calls and increment it
        //printf("%d %s\n", num_calls, get_token_keep(token_storage)->value);
    }
    num_calls++;
    //get_token(token_storage);
    //return 1;
    int left_brackets = 0;
    list_t *list = list_init();
    list_insert_first(&list, DOLLAR);
    symbol_enum input;
    symbol_enum top;
    bool valid;

    symbol_enum symbol1 = INT;
    symbol_enum symbol2 = INT;
    symbol_enum symbol3 = INT;
    int numsym;

    int error = 0;

    assoc_t prec_operator; 
    bool input_loaded = false;

    while (!input_loaded || !(final_condition(list))) { //TODO 
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
            top = list_get_first_term(&list);
            input = convert_token_to_symbol(token,&valid);
            if (valid == 0){
                input_loaded = 1;
                continue;
            }
        }
        else {
            input = DOLLAR;
            top = list_get_first_term(&list);
        }
        // tuto dame ten oprator ktory budeme musiet pozuit, takze bud <, >, =, x, alebo koniec
        prec_operator = prec_table[convert_symbol_to_int(top)][convert_symbol_to_int(input)];
        printf("top je %d, input je %d, operator je %d, konvertovane: %d, %d, realny token je %s %d\n", top, input, prec_operator, convert_symbol_to_int(top), convert_symbol_to_int(input), get_token_keep(token_storage)->value, input_loaded == true);
        // na zaklade toho aky je to operator tak rozhodneme co budeme robit 
        if (prec_operator == L_A) {        
            if(!return_before_stop(&list, &symbol1, &symbol2, &symbol3, &numsym)){
                error = 0;
                goto end;
            }
            if(rule_check(&symbol1, &symbol2, &symbol3, numsym))
            {        
                list_insert_first(&list, NONTERM);
                print_list(list);
                if (input_loaded && final_condition(list)) {
                    error = 1;
                    goto end;
                }
            }
            else {
                error = 0;
                goto end;
            }
        }
        // <
        else if (prec_operator == R_A) {   
            list_insert_after_nonterm(&list);
            list_insert_first(&list, input);
            print_list(list);
            if(!input_loaded) {
                get_token(token_storage);
            }

           
        }
        else if (prec_operator == EQ_A) {
            list_insert_first(&list, input);
            print_list(list);
            if(!input_loaded) {
                get_token(token_storage);
            }


        }
        else if (prec_operator == ERR) {
            error = 0;
            goto end;
        }
        else if (prec_operator == END) {
            error = 1;
            goto end;
        }
    }
    end:
    list_free(list);
    return error;
} 


int convert_symbol_to_int(symbol_enum symbol) {
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

symbol_enum convert_token_to_symbol(token_t *token, bool *valid) {
    *valid = 1;
    if (token == NULL) {
        *valid = 0;
        return INT;
    }

    if (((token->token_type == TOK_ID) && (token->value[0] == '$')) || token->token_type == TOK_LIT) {
        return INT; // sem string alebo int alebo float
    }

    if(token->token_type == TOK_ID) {
        // toto je funkcia
        // takze s tym nieco este budeme musiet spravit
        return INT;
    }

    int length = strlen(token->value);
    if (length == 1) {
        switch (token->value[0]) {
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '.':
            return CONC;
        case '<':
            return LT;
        case '>':
            return GT;
        case '(':
            return OPENBR;
        case ')':
            return CLOSEDBR;
        }
    }
    else {
        if (strcmp(token->value, "<=") == 0) {
            return LTE;
        }
        if (strcmp(token->value, ">=") == 0) {
            return GTE;
        }
        if (strcmp(token->value, "===") == 0) {
            return EQ;
        }
        if (strcmp(token->value, "!==") == 0) {
            return NEQ;
        }
    }
    *valid = 0;
    return NEQ;
}

int rule_check(symbol_enum* symbol1, symbol_enum* symbol2, symbol_enum* symbol3, int numsym) {
    if (numsym == 1) {
        if (*symbol1 == INT || *symbol1 == FLOAT || *symbol1 == STRING) {
            printf("Pouzivam pravidlo ked numsym je jedna\n");
            return 1;
        }
    }
    if (numsym == 3)
    {
        if (*symbol1 == NONTERM && *symbol2 < DOLLAR && *symbol3 == NONTERM) {
            printf("Pouzival pravidlo operator\n");
            return 1; 
            //  TODO dd pouzijeme pravidlo E op E je E neviem ako to pouzijeme thb
            //  toto asi este rozpiseme pre jeednotlive operacia aby sme spravili ten storm 
        }
        if (*symbol1 == OPENBR && *symbol2 == NONTERM && *symbol3 == CLOSEDBR) {
            printf("Pouzival pravidlo operator\n");
            return  1; 
            //  TODO dd pouzijeme pravidlo (E) je E neviem ako to pouzijeme thb
        }
    }
    return 0;
}