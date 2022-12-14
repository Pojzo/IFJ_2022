/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "parser.h"
#include "expression.h"
#include "utils.h"
#include "symtable.h"

int token_index = 0;
extern const char *prolog;
extern const int DEBUG_PARSER;
extern const int DEBUG_LEXER;
extern const char *epilog;
extern const char* string_operators[];
extern const int string_oper_len;

//arguments for inserting into symtable
char *scope = "global";
char *return_type = "";
char **args = NULL;

extern int error;

id_node_t *id_node = NULL;

//definicia symbol table


int parser_start(char *buffer) {
    token_index = 0;
    // we first check if <? is present

    token_storage_t *token_storage = token_storage_create();
    //int error = 0;
    if (!check_prolog(buffer)) {
        error = 2;
        goto end;
    }
    buffer += strlen(prolog);

    error = dka(buffer, strlen(buffer), token_storage);

    // check if there was an error in lexical analysis
    if (DEBUG_LEXER) {
        token_t *token = NULL;
        while ((token = get_token(token_storage)) != NULL) {
        }
    }
    token_index = 0;
    if (error) {
        error = 1;
        goto end;
    }

    else {
    }
    add_builtin_functions();
    // check if declare(strict_types=1) is present
    if (!rule_program(token_storage)) {
        if (error == 0) {
            error = 2;
        }
        goto end;
    }

    else {
    }

    end:
    token_storage_free(token_storage);
    free_tree(id_node);
    return error;
}

// check if buffer starts with prolog
bool check_prolog(char *buffer) {
    for (size_t i = 0; i < strlen(prolog); i++) {
        if (buffer[i] != prolog[i]) {
            return false;
        }
    }
    return true;
}

bool check_epilog(char *buffer) {
    for (size_t i = 0; i < strlen(epilog); i++) {
        if (buffer[i] != epilog[i]) {
            return false;
        }
    }
    return true;
}
//-----------------------------------------------------------------------------------------------------------
//RULES

//<program>-> <prolog> <stlist> 
bool rule_program(token_storage_t *token_storage) {
    return check_strict_types(token_storage) && rule_stlist(token_storage);
}

//checking whether prolog is complete
int check_strict_types(token_storage_t *token_storage) {
    token_t *token = get_token(token_storage);

    if (token == NULL || token->token_type != TOK_ID || strcmp(token->value, "declare") != 0) {
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, "(") != 0) {
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_ID || strcmp(token->value, "strict_types") != 0) {
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_OPERATOR || strcmp(token->value, "=") != 0) {
        return 0;
    }
    
    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_LIT || strcmp(token->value, "1") != 0) {
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, ")") != 0) {
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, ";") != 0) {
        return 0;
    }
    return 1;
}

bool rule_stlist(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    //<stlist> -> <EOF> 
    if (token == NULL)
        return 1;
    //<stlist> -> <epilog> 
    if(token->token_type == TOK_EPILOG )
        return 1;
    //<stlist> -> <statement> <stlist>
    if (rule_st(token_storage)) {
        if (rule_stlist(token_storage)) {
            return 1;
        }
    }
    if (rule_fdef(token_storage)) {
        if (rule_stlist(token_storage)){
            return 1;
        }
    }
    return 0;
}

bool term_if(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD && strcmp(token->value, "if") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}


bool term_while(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD && strcmp(token->value, "while") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_open_bracket(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, "(") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_close_bracket(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, ")") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_open_curly_bracket(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, "{") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_close_curly_bracket(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, "}") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_else(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD && strcmp(token->value, "else") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}


bool term_function(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD && strcmp(token->value, "function") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_equals(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_OPERATOR && strcmp(token->value, "=") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_semicolon(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, ";") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_colon(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, ":") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_comma(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_SEPARATOR && strcmp(token->value, ",") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_return(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD && strcmp(token->value, "return") == 0) {
        get_token(token_storage);
        return 1;
    }
    return 0;
}


bool term_id(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_ID && (token->value)[0] == '$') {
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_idfun(token_storage_t *token_storage, bool is_fdef) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_ID && (token->value)[0] != '$') {
        if(is_fdef) {
            scope = token->value;
        }
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_type(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD) {
        if(strcmp(token->value, "int") == 0 || strcmp(token->value, "string") == 0 || strcmp(token->value, "float") == 0 || strcmp(token->value, "void") == 0
           || strcmp(token->value, "?int") == 0 || strcmp(token->value, "?string") == 0 || strcmp(token->value, "?float") == 0) {
            //char *return_type = token->value;
            get_token(token_storage);
            return 1;
        }    
    }
    return 0;
}

bool term_idfun_call(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token == NULL) {
        return false;
    }
    if (token->token_type == TOK_ID && token->value[0] != '$') {
        if (search(id_node, token->value) != NULL) {
            get_token(token_storage);
            return true;
        }
        error = 3;
        return false;
        // get_token(token_storage);
        // return true;
    }
    return false;
}

bool rule_st(token_storage_t *token_storage) {
    // default datatype
    datatype_t datatype = TYPE_VOID;
    token_t *token = get_token_keep(token_storage);
    if (term_while(token_storage)) {
        if (term_open_bracket(token_storage) && rule_expr(token_storage, &datatype, 0) && 
               term_close_bracket(token_storage) && term_open_curly_bracket(token_storage) && rule_fstlist(token_storage)) {
            return 1;
        }
        return 0;
    }
    
    if (term_if(token_storage)) {
        if (term_open_bracket(token_storage) && rule_expr(token_storage, &datatype, 0) && term_close_bracket(token_storage)
        && term_open_curly_bracket(token_storage) && rule_fstlist(token_storage)) {
            token = get_token_keep(token_storage);
            if (token != NULL && token->token_type == TOK_KEYWORD && strcmp(token->value, "else") == 0) {
                get_token(token_storage);
                if (rule_else(token_storage)) {
                    return 1;      
                }
            } 
            else {
                return 1;
            }
         
        }
        return 0;
    }

    if (term_idfun_call(token_storage)) {
        if(term_open_bracket(token_storage) && rule_funccallarg(token_storage, token->value)) {
            return 1;
        }
        else {
            return 0;
        }
    }

    if (term_id(token_storage)) {
        if( term_equals(token_storage) && rule_expr(token_storage, &datatype, 0) && 
        term_semicolon(token_storage)) {
            insert_id(&id_node, token->value, datatype, scope);
            return 1;
        }
        else {
            return 0;
        }
    }
    if (term_return(token_storage)) {
        if (rule_return_cond(token_storage)) {
            return 1;
        }
    }
    return 0;  
}

bool rule_fstlist(token_storage_t *token_storage) {
    if (term_close_curly_bracket(token_storage)) {
        return 1;
    }
    if (rule_st(token_storage) && rule_fstlist(token_storage)) {
        return 1;
    }

    return 0;
}

bool rule_else (token_storage_t *token_storage) {
    if (term_open_curly_bracket(token_storage) && rule_fstlist(token_storage)) {
        return 1;
    }
    return 0;
}

bool rule_funccallarg(token_storage_t *token_storage, char *function_name) {
    id_node_t *function = search(id_node, function_name);
    int anticipated_args = function->num_arguments;
    if (term_close_bracket(token_storage) && term_semicolon(token_storage)) {
        if (anticipated_args != 0) {
            error = 4;
            return 0;
        }
        return 1;
    }
    datatype_t datatype = TYPE_VOID;
    if (rule_expr(token_storage, &datatype, 1)) {
        if (function->num_arguments != 0 && function->arguments[0] == TYPE_OPT_INT && (datatype == TYPE_INT || datatype == TYPE_VOID)) {
            goto skip;
        }
        if (function->num_arguments != 0 && function->arguments[0] == TYPE_OPT_FLOAT && (datatype == TYPE_FLOAT || datatype == TYPE_VOID)) {
            goto skip;
        }

        if (function->num_arguments != 0 && function->arguments[0] == TYPE_OPT_STRING && (datatype == TYPE_STRING || datatype == TYPE_VOID)) {
            goto skip;
        }

        if (strcmp("write", function_name) != 0 && datatype != function->arguments[0]) {
            error = 4;
            return 0;
        }
 skip:
        if (rule_next(token_storage, anticipated_args, 1, function) && term_semicolon(token_storage)) {
            return 1;
        }
    }
    return 0;
}

bool rule_next (token_storage_t *token_storage, int anticipated_args, int curr_args, id_node_t *function) {
    if (term_close_bracket(token_storage)) {
        if(curr_args != anticipated_args && strcmp(function->name, "write") != 0) {
            error = 4;
            return 0;
        }
        return 1;
    }
    datatype_t datatype = TYPE_VOID;
    if (term_comma(token_storage)) {
        if (rule_expr(token_storage, &datatype, 0)) {
            if (function->num_arguments > curr_args && function->arguments[curr_args] == TYPE_OPT_INT && (datatype == TYPE_INT || datatype == TYPE_VOID)) {
                goto skip;
            }

            if (function->num_arguments > curr_args && function->arguments[curr_args] == TYPE_OPT_FLOAT && (datatype == TYPE_FLOAT || datatype == TYPE_VOID)) {
                goto skip;
            }

            if (function->num_arguments > curr_args && function->arguments[curr_args] == TYPE_OPT_STRING && (datatype == TYPE_STRING || datatype == TYPE_VOID)) {
                goto skip;
            }
            if (strcmp("write", function->name) != 0 && datatype != function->arguments[curr_args]) {
                error = 4;
                return 0;
            }
skip:
            if (rule_next(token_storage, anticipated_args, curr_args + 1, function)) {
                return 1;
            }
        }
    }
    return 0;
}

bool rule_fdef (token_storage_t *token_storage) {
    if (term_function(token_storage)) {
        token_t *fun_name = get_token_keep(token_storage);
        if (insert_function_id(&id_node, fun_name->value) == 3) {
            error = 3;
            return 0;
        }

        if (term_idfun(token_storage, true) && term_open_bracket(token_storage) && rule_function_arguments(token_storage) && 
                term_colon(token_storage)) {

            token_t *token = get_token_keep(token_storage);
            fun_add_return_type(id_node, fun_name->value, convert_char_to_datatype(token->value));
            if(term_type(token_storage) && term_open_curly_bracket(token_storage) &&
                    rule_function_body(token_storage)) {
                //insert_function_id(id_node, scope, return_type, arguments);
                scope = "global";
                return 1;
            }
        }
    }
    return 0;
}

bool rule_function_arguments(token_storage_t *token_storage) {
    if (term_close_bracket(token_storage)) { //tu sme
        return 1;
    }
    if(rule_argf(token_storage) && rule_more_argf(token_storage)) {
        return 1;
    }
    return 0;
}

//rule argf
bool rule_argf(token_storage_t *token_storage) {
    token_t *datatype = get_token_keep(token_storage);
    if (term_type(token_storage)){
        token_t *name = get_token_keep(token_storage);
        if(term_id(token_storage)) {
            insert_param_id(&id_node, name->value, convert_char_to_datatype(datatype->value), scope);
            fun_add_arg(id_node, scope, convert_char_to_datatype(datatype->value));
            return 1;
        }
    }
    return 0;
}



bool rule_more_argf(token_storage_t *token_storage) {
    if (term_close_bracket(token_storage)) {
        return 1;
    }
    if(term_comma(token_storage) && rule_argf(token_storage) && rule_more_argf(token_storage)) {
        return 1;
    }
    return 0;
}

bool rule_function_body(token_storage_t* token_storage) {
    if( term_open_bracket(token_storage)) {
        return 1;
    }
    if(rule_fstlist(token_storage)) {
        return 1;
    }
    return 0;
}

bool rule_return_cond(token_storage_t* token_storage) {
    datatype_t return_datatype = search_return_type(id_node, scope);
    datatype_t expr_datatype = TYPE_VOID;
    if (term_semicolon(token_storage)) {
        if (return_datatype != TYPE_VOID && strcmp(scope, "global") != 0) {
            error = 4;
            return 0;
        }
        return 1;
    }

    if (rule_expr(token_storage, &expr_datatype, 1) && term_semicolon(token_storage)) {
        if (strcmp(scope, "global") == 0) {
            return 1;
        }
        if (return_datatype == TYPE_OPT_INT) {
            if (expr_datatype == TYPE_INT || expr_datatype == TYPE_VOID) {
                return 1;
            }
            error = 4;
            return 0;
        }

        if (return_datatype == TYPE_OPT_FLOAT) {
            if (expr_datatype == TYPE_FLOAT || expr_datatype == TYPE_VOID) {
                return 1;
            }
            error = 4;
            return 0;
        }

        if (return_datatype == TYPE_OPT_STRING) {
            if (expr_datatype == TYPE_STRING || expr_datatype == TYPE_VOID) {
                return 1;
            }
            error = 4;
            return 0;
        }       

        if (return_datatype != expr_datatype) {
            error = 4;
            return 0;
        }
        return 1;
    }
    return 0;
}

void add_builtin_functions() {
    insert_function_id(&id_node, "reads");
    insert_function_id(&id_node, "readi");
    insert_function_id(&id_node, "readf");
    insert_function_id(&id_node, "write");

    insert_function_id(&id_node, "floatval");
    fun_add_arg(id_node, "floatval", TYPE_INT);
    fun_add_return_type(id_node, "floatval", TYPE_FLOAT);


    insert_function_id(&id_node, "intval");
    fun_add_arg(id_node, "intval", TYPE_STRING);
    fun_add_return_type(id_node, "intval", TYPE_INT);

    insert_function_id(&id_node, "strval");
    fun_add_arg(id_node, "strval", TYPE_INT);
    fun_add_return_type(id_node, "strval", TYPE_STRING);

    insert_function_id(&id_node, "strlen");
    fun_add_arg(id_node, "strlen", TYPE_STRING);
    fun_add_return_type(id_node, "strlen", TYPE_INT);

    insert_function_id(&id_node, "substring");
    fun_add_arg(id_node, "substring", TYPE_STRING);
    fun_add_arg(id_node, "substring", TYPE_INT);
    fun_add_arg(id_node, "substring", TYPE_INT);

    fun_add_return_type(id_node, "substring", TYPE_OPT_STRING);

    insert_function_id(&id_node, "ord");
    fun_add_arg(id_node, "ord", TYPE_STRING);
    fun_add_return_type(id_node, "ord", TYPE_INT);

    insert_function_id(&id_node, "chr");
    fun_add_arg(id_node, "chr", TYPE_INT);
    fun_add_return_type(id_node, "chr", TYPE_STRING);

    return;
}

