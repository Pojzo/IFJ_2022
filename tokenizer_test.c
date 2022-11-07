#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

int main() {
    char *buffer = "gazdik";
    token_storage_t *token_storage = token_storage_create();
    token_storage_add(token_storage, TOK_ID, buffer, 2);
    token_storage_add(token_storage, TOK_KEYWORD, buffer, 3);
    token_storage_add(token_storage, TOK_SEPARATOR, buffer, 4);
    token_storage_add(token_storage, TOK_LITERAL, buffer, 5);
    for(int i = 0; i < 4; i++) {
        token_print(token_storage->tokens[i]);
    }
    printf("Tokenizer->num_tokens: %d\nTokenizer->array_len %d\n", token_storage->num_tokens, token_storage->array_len);
    token_storage_free(token_storage);
    // dka(buffer, buffer_len, tokens);
    return 0;
}
