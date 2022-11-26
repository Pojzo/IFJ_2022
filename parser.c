#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"


int token_index = 0;

int parser_start(char *buffer) {
    token_storage_t *token_storage = token_storage_create();
    int error = dka(buffer, strlen(buffer), token_storage);

    token_t *token = NULL;
    
    while ((token = get_token(token_storage)) != NULL) {
        token_print(token);
    }
    printf("Number of tokens: %d\n", token_storage->num_tokens);

    token_storage_free(token_storage);
    if (error) {
        printf("[ERROR] An error has occured in lexical analysis %s\n", "\U0001F913");
    }
    return error;
}

token_t *get_token(token_storage_t *token_storage){
    if (token_index < token_storage->num_tokens) {
        return token_storage->tokens[token_index++];
    }
    else {
        // jak si to vedel ty prijebany
        return NULL;
    }
}