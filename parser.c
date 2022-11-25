#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

token_storage_t *token_storage;
int token_index = 0;

void start() {
    token_storage_create (&token_storage);
    printf("Hello world!\n");
}

token_t *get_token(int i){
    if (i < token_storage->num_tokens) {
        return token_storage->tokens[i];
    }
    else {
        // jak si to vedel ty prijebany
        return NULL;
    }
}