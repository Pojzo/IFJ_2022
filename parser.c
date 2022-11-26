#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"


int token_index = 0;

int parser_start(char *buffer) {
    token_storage_t *token_storage = token_storage_create();
    int error = dka(buffer, strlen(buffer), token_storage);

    // check if there was an error in lexical analysis
    if (error) {
        printf("[ERROR] An error has occured in lexical analysis %s\n", "\U0001F913");
        return 1;
    }

    // check if declare(strict_types=1) is present
    if (check_strict_types(token_storage)) {
        printf("[ERROR] An error has occured in syntax analysis %s\n", "\U0001F913");
        return 2;
    }

    token_t *token = NULL;
    
    while ((token = get_token(token_storage)) != NULL) {
        token_print(token);
    }
    printf("Number of tokens: %d\n", token_storage->num_tokens);

    token_storage_free(token_storage);

    return error;
}

// get the next token from token_storage
token_t *get_token(token_storage_t *token_storage){
    if (token_index < token_storage->num_tokens) {
        return token_storage->tokens[token_index++];
    }
    else {
        // jak si to vedel ty prijebany
        return NULL;
    }
}

//checking whether prolog is complete
bool check_strict_types(token_storage_t *token_storage) {
    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_ID || strcmp(token->value, "declare") != 0) {
        return false;
    }

    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_SEP || token->value != '(') {
        return false;
    }

    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_ID || strcmp(token->value, "strict_types") != 0) {
        return false;
    }

    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_OP || token->value != '=') {
        return false;
    }

    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_LIT || strcmp(token->value, "1") != 0) {
        return false;
    }

    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_SEP || token->value != ')') {
        return false;
    }

    token_t *token = get_token(token_storage);
    if (token == NULL || token->type != TOK_SEP || token->value != ';') {
        return false;
    }

    return true;
}