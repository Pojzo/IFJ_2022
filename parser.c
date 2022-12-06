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
int error = 0;
int redefinition = 0;
extern const char *prolog;
extern const int DEBUG_PARSER;
extern const int DEBUG_LEXER;
extern const char *epilog;
extern const char* string_operators[];
extern const int string_oper_len;

//arguments for inserting into symtable
char *scope = "global"; //viem ze take tu neni, ale defaultne som to nazval tak, ked premenna nie je vo funkcii
char *return_type = "";
char **args = NULL;

id_node_t *id_node = NULL;

//definicia symbol table


int parser_start(char *buffer) {
    token_index = 0;
    // we first check if <? is present

    token_storage_t *token_storage = token_storage_create();
    //int error = 0;
    if (!check_prolog(buffer)) {
        printf("\x1b[31m" "Error in checking prolog" "\x1b[0m" "\n");
        error = 2;
        goto end;
    }
    buffer += strlen(prolog);

    error = dka(buffer, strlen(buffer), token_storage);

    // check if there was an error in lexical analysis
    if (DEBUG_LEXER) {
        token_t *token = NULL;
        while ((token = get_token(token_storage)) != NULL) {
        token_print(token);
        }
    }
    token_index = 0;
    if (error) {
        printf("[ERROR] An error has occured in lexical analysis %s\n", "\U0001F913");
        error = 1;
        goto end;
    }

    else {
        printf("\x1b[32m" "\nLexical analysis was successful\n" "\x1b[0m");
    }

    /*insert_function_id(&id_node, "floatval");
    insert_function_id(&id_node, "intval");
    insert_function_id(&id_node, "strval");
    insert_function_id(&id_node, "strlen");
    insert_function_id(&id_node, "substring");*/
    // check if declare(strict_types=1) is present
    if (!rule_program(token_storage)) {
        printf("\x1b[31m" "Error in rule_program" "\x1b[0m" "\n");
        if (redefinition != 0){
            error = 3;
        }
        else{
            error = 2;
        }
        goto end;
    }

    else {
        printf("\x1b[32m" "Syntax analysis was successful\n" "\x1b[0m");
    }

    // printf("Number of tokens: %d\n", token_storage->num_tokens);
    end:
    if (DEBUG_PARSER) print_tree(id_node);
    token_storage_free(token_storage);
    free_tree(id_node);
    return 1;
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
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in first condition" "\x1b[0m" "\n");
        printf("%d %d %d\n", token == NULL, token_index, token_storage->num_tokens);
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, "(") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in second condition" "\x1b[0m" "\n");
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_ID || strcmp(token->value, "strict_types") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in third condition" "\x1b[0m" "\n");
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_OPERATOR || strcmp(token->value, "=") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in fourth condition" "\x1b[0m" "\n");
        return 0;
    }
    
    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_LIT || strcmp(token->value, "1") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in fifth condition" "\x1b[0m" "\n");
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, ")") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in sixth condition" "\x1b[0m" "\n");
        return 0;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, ";") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in seventh condition" "\x1b[0m" "\n");
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
             if (DEBUG_PARSER) printf("the scope is: %s\n", scope);
        }
        get_token(token_storage);
        return 1;
    }
    return 0;
}

bool term_type(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (token != NULL && token->token_type == TOK_KEYWORD) {
        if(strcmp(token->value, "int") == 0 || strcmp(token->value, "string") == 0 || strcmp(token->value, "float") == 0 || strcmp(token->value, "void") == 0) {
            //char *return_type = token->value;
            get_token(token_storage);
            return 1;
        }    
    }
    return 0;
}


bool rule_st(token_storage_t *token_storage) {
    token_t *token = get_token_keep(token_storage);
    if (term_while(token_storage)) {
        if (term_open_bracket(token_storage) && rule_expr(token_storage) && 
               term_close_bracket(token_storage) && term_open_curly_bracket(token_storage) && rule_fstlist(token_storage)) {
            return 1;
        }
        return 0;
    }
    
    if (term_if(token_storage)) {
        if (term_open_bracket(token_storage) && rule_expr(token_storage) && term_close_bracket(token_storage)
        && term_open_curly_bracket(token_storage) && rule_fstlist(token_storage)) {
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

    if (term_idfun(token_storage, 0)) {
        
        if(term_open_bracket(token_storage) && rule_funccallarg(token_storage)) {
            return 1;
        }
        else {
            return 0;
        }
    }

    if (term_id(token_storage)) {
        
        insert_id(&id_node, token->value, TYPE_VOID, scope);

        //print_tree(id_node);
        if( term_equals(token_storage) && rule_expr(token_storage) && 
        term_semicolon(token_storage)) {
            if (DEBUG_PARSER) printf("the scope is: %s\n", scope);
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

bool rule_funccallarg (token_storage_t *token_storage) {
    if (term_close_bracket(token_storage) && term_semicolon(token_storage)) {
        return 1;
    }
    if (rule_expr(token_storage) && rule_next(token_storage) && term_semicolon(token_storage)) {
        return 1;
    }
    return 0;
}

bool rule_next (token_storage_t *token_storage) {
    if (term_close_bracket(token_storage)) {
        return 1;
    }
    if(term_comma(token_storage) && rule_expr(token_storage) && rule_next(token_storage)) {
        return 1;
    }
    return 0;
}

bool rule_fdef (token_storage_t *token_storage) {
    if (term_function(token_storage)) {
        token_t *fun_name = get_token_keep(token_storage);
        // toto znamena ze tato funkcia uz bola definovana
        if (insert_function_id(&id_node, fun_name->value) == 3) {
            printf("sme tu brasko vydrbalo nas\n");
            redefinition = 3;
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
                if (DEBUG_PARSER) printf("the scope is: %s\n", scope);
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
            insert_id(&id_node, name->value, convert_char_to_datatype(datatype->value), scope);
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
    if (term_semicolon(token_storage)) {
        return 1;
    }
            
    if (rule_expr(token_storage) && term_semicolon(token_storage)) {
        return 1;
    }
    return 0;
}
