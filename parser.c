#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenizer.h"
#include "parser.h"


int token_index = 0;
extern const char *prolog;
extern const int DEBUG_PARSER;
extern const char *epilog;

int parser_start(char *buffer) {
    token_index = 0;
    // we first check if <? is present

    if (!check_prolog(buffer)) {
        printf("\x1b[31m" "Error in checking prolog" "\x1b[0m" "\n");
        return 2;
    }
    buffer += strlen(prolog);

    token_storage_t *token_storage = token_storage_create();
    int error = dka(buffer, strlen(buffer), token_storage);

    // check if there was an error in lexical analysis
    
    if (DEBUG_PARSER) {
        token_t *token = NULL;
        while ((token = get_token(token_storage)) != NULL) {
        token_print(token);
        }
    }
    token_index = 0;
    if (error) {
        printf("[ERROR] An error has occured in lexical analysis %s\n", "\U0001F913");
        return error;
    }

    // check if declare(strict_types=1) is present
    if (rule_program(token_storage)) {
        printf("\x1b[31m" "Error in rule_program" "\x1b[0m" "\n");
        return 2;
    }

    // printf("Number of tokens: %d\n", token_storage->num_tokens);

    token_storage_free(token_storage);

    return error;
}

// get the next token from token_storage
token_t *get_token(token_storage_t *token_storage){
    if (token_index < token_storage->num_tokens) {
        return token_storage->tokens[token_index++];

    }
    else {
        return NULL;
    }
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
    return check_strict_types(token_storage); // || rule_stlist(token_storage);
}

//checking whether prolog is complete
int check_strict_types(token_storage_t *token_storage) {
    token_t *token = get_token(token_storage);

    if (token == NULL || token->token_type != TOK_ID || strcmp(token->value, "declare") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in first condition" "\x1b[0m" "\n");
        printf("%d %d %d\n", token == NULL, token_index, token_storage->num_tokens);
        return 1;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, "(") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in second condition" "\x1b[0m" "\n");
        return 1;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_ID || strcmp(token->value, "strict_types") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in third condition" "\x1b[0m" "\n");
        return 1;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_OPERATOR || strcmp(token->value, "=") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in fourth condition" "\x1b[0m" "\n");
        return 1;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_LIT || strcmp(token->value, "1") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in fifth condition" "\x1b[0m" "\n");
        return 1;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, ")") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in sixth condition" "\x1b[0m" "\n");
        return 1;
    }

    token = get_token(token_storage);
    if (token == NULL || token->token_type != TOK_SEPARATOR || strcmp(token->value, ";") != 0) {
        if (DEBUG_PARSER) printf("\x1b[31m" "Error in seventh condition" "\x1b[0m" "\n");
        return 1;
    }

    return 0;
}

/*
bool rule_stlist(token_storage_t *token_storage) {
    token_t *token = get_token(token_storage);
    //<stlist> -> <EOF> 
    if (token == NULL);
        retrun 0;
    //<stlist> -> <epilog> 
    if(token->token_type == TOK_EPILOG );
        return 0;
    //<stlist> -> <statement> <stlist>
    if (rule_st(token)) {
        if (stlist(token_storage)) {
            return 0;
        }
        return 1;
    }
    return 1;
}

bool rule_st()
*/
