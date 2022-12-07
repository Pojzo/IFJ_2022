
/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "tokenizer.h"
#include "list.h"
#include "symtable.h"

typedef enum {
    L_A, // left association >
    R_A, // right association <
    EQ_A, // equal association = 
    ERR,  // error
    END   // end of input
} assoc_t;

int error = 0;

#define N 8

extern id_node_t *id_node;
extern char *scope;

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
	{ R_A , R_A , R_A ,  R_A  , R_A ,  ERR  , R_A , END }  /// $
};

bool moved_input;
extern bool rule_st;

bool rule_expr(token_storage_t *token_storage, datatype_t *datatype) {
    printf("tu soms a dostal more gadzo\n");
    symbol_enum type = NONTERM;
    moved_input = true;
    int left_brackets = 0;
    list_t *list = list_init();
    int inner_error = false;
    while(true) {
        // get current token
        token_t *token = get_token_keep(token_storage);
        if (moved_input) {
            int end = check_end(token, &left_brackets);
            if (end == 1) {
                // end of input
                // second part of expression, $ is always on input
                inner_error = rule_expr2(&list, token_storage);
                goto end;
            }
        }

        bool valid = 1;
        symbol_enum top = list_get_first_term(list);
        symbol_enum input = convert_token_to_symbol(token, &valid);
        if (input == INT || input == FLOAT || input == STRING) {
            if (type == NONTERM) {
                if () // TODO
            }
        }

        if (token->token_type == TOK_ID && token->value[0] == '$') {
            if (!check_if_declared(id_node, token->value, scope)) {
                // printf("toto sa malo stat\n");
                error = 5;
                return false;
            }
        }

        //TODO tu spravime aby mali aj value 

        // we need to check if input was valid
        if (valid == 0) {
            // syntax error
            inner_error = false;
            goto end;
        }
        if (!main_alg(&list, top, input, token_storage, 0)) {
            // this mean that there has been an error in main_alg
            inner_error = false;
            goto end;
        }
    }
end:
    list_free(list);
    // printf("Pocet zatvoriek na konci %d\n", left_brackets);
    return inner_error;
}

// second part of expression when input end
bool rule_expr2(list_t **list, token_storage_t* token_storage) {
    while (true) {
        symbol_enum top = list_get_first_term(*list);
        int ret_code = main_alg(list, top, DOLLAR, token_storage, 1);
        if (ret_code == 2) {
            // if end
            return 1;
        }
        else if (ret_code == 1) {
            continue;
        }
        else {
            return 0;
        }
    }
}

int main_alg(list_t **list, symbol_enum top, symbol_enum input, token_storage_t *token_storage, bool input_ended) {
    // printf("Printujem list, ked input %s toto je top: %d, toto je input: %d  \n", input_ended == true ? "skoncil" : "neskoncil", top, input);
    // print_list(*list);
    // get precedence operator from table
    int row = convert_symbol_to_int(top); 
    int col = convert_symbol_to_int(input);

    assoc_t prec_operator = prec_table[row][col];
    if (prec_operator == ERR) {
        // error in finding prec_operator
        return 0;
    }
    // <
    if (prec_operator == R_A) {
        right_assoc(list, input);
        moved_input = true;
        if (!input_ended) {
            get_token(token_storage);
        }
        return 1;
    }
    else if (prec_operator == L_A) {
        moved_input = false;
        return left_assoc(list);
    }
    else if (prec_operator == EQ_A) {
        moved_input = true;
        eq_assoc(list, input);
        if (!input_ended) {
            get_token(token_storage);
        }        
        return 1; 
    }
    // else if (prec_operator == END) {
    return 2; 
}

void eq_assoc(list_t **list, symbol_enum input) {
    list_insert_first(list, input);
}

// <
void right_assoc(list_t **list, symbol_enum input) {
    // find the first nonterm and insert < after it 
    list_insert_after_nonterm(list);

    // push input on top of the stack
    list_insert_first(list, input);

    // called function will read the next symbol
}

// > return false if there was an error in the algorithm
bool left_assoc(list_t **list) {
    return rule_check(list);
}

bool rule_check(list_t **list) {
    int num_till_stop = symbols_till_stop(*list);
    // printf("Viktor stromcek: %d\n", num_till_stop);
    if (num_till_stop == 1) {
        // pouzijeme i -> E
        list_pop_first(list);
        list_pop_first(list);
        list_insert_first(list, NONTERM);
        return 1;
    }
    else if (num_till_stop == 3) {
        list_pop_first(list);
        list_pop_first(list);
        list_pop_first(list);
        list_pop_first(list);        
        list_insert_first(list, NONTERM);
        return 1;
    }
    return 0;    
}

bool check_end(token_t *token, int *left_brackets) {
    if (strcmp(token->value, "(") == 0) {
        // printf("Pridavam lavu zatvorku\n");
        (*left_brackets)++;
        // this is good
        return 0;
    }
    if (strcmp(token->value, ")") == 0) {
        (*left_brackets)--;
        // printf("Uberam pravu zatvorku\n");
        if (*left_brackets < 0) {
            // this is bad, meaning we're reached the end of expr
            return 1;
        }
        return 0;
    }
    if (strcmp(token->value, ",") == 0) {
        return 1;
    }
    if (strcmp(token->value, ";") == 0) {
        return 1;
    }
    return 0;
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

    if ((token->token_type == TOK_ID) && (token->value[0] == '$')){
        return INT;
        //return (get_data_t(token));
    } 

    if (token->token_type == TOK_LIT) {
        if (token->value[0] == '"') {
            return STRING;
        }
        else if (contains_dot(token->value)) {
            return FLOAT;
        }
        else {
            return INT;
        }
    }

    if(token->token_type == TOK_ID) {
        // TODO tunkcia 
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

datatype_t token_to_datatyep

/*
bool rule_funccallarg (token_storage_t *token_storage, char *function_name) {
    id_node_t *function = search(id_node, function_name);
    int anticipated_args = function->num_arguments;
    if (term_close_bracket(token_storage) && term_semicolon(token_storage)) {
        if (anticipated_args != 0) {
            error = 4;
            return 0;
        }
        return 1;
    }
    if (rule_expr(token_storage) && rule_next(token_storage, anticipated_args, 1, function_name) && term_semicolon(token_storage)) {
        return 1;
    }
    return 0;
}

bool rule_next (token_storage_t *token_storage, int anticipated_args, int curr_args, char *function_name) {
    if (term_close_bracket(token_storage)) {
        if(curr_args != anticipated_args && strcmp(function_name, "write") != 0) {
            error = 4;
            return 0;
        }
        return 1;
    }
  if (term_idfun_call(token_storage)) {
        if(term_open_bracket(token_storage) && rule_funccallarg(token_storage, token->value)) {
            return 1;
        }
        else {
            return 0;
        }
    }


*/
