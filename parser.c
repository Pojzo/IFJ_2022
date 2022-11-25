#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

// token_storage_t *token_storage;
int token_index = 0;

int parser_start(char *buffer) {
    token_storage_t *token_storage = token_storage_create();
    int error = dka(buffer, strlen(buffer), token_storage);
        for(int i = 0; i < token_storage->num_tokens; i++) {
    token_print(token_storage->tokens[i]);
    }
    printf("Length of token storage -> %d\n", token_storage->num_tokens);

    token_storage_free(token_storage);
    if (error) {
        printf("[ERROR] An error has occured in lexical analysis %s\n", "\U0001F913");
    }

    return 0;
}

/*
token_t *get_token(int i){
    if (i < token_storage->num_tokens) {
        return token_storage->tokens[i];
    }
    else {
        // jak si to vedel ty prijebany
        return NULL;
    }
}
*/