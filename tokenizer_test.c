#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

int main() {
    char *buffer = "                    $gazdik;;;  $kokot    ;";
    token_storage_t *token_storage = token_storage_create();
    /*
    token_storage_add(token_storage, TOK_ID, buffer, 2);
    token_storage_add(token_storage, TOK_KEYWORD, buffer, 3);
    token_storage_add(token_storage, TOK_SEPARATOR, buffer, 4);
    token_storage_add(token_storage, TOK_LITERAL, buffer, 5);
    printf("Tokenizer->num_tokens: %d\nTokenizer->array_len %d\n", token_storage->num_tokens, token_storage->array_len);
    for(int i = 0; i < 4; i++) {
        token_print(token_storage->tokens[i]);
    }

    */
    int error = dka(buffer, strlen(buffer), token_storage);
    for(int i = 0; i < token_storage->array_len; i++) {
        token_print(token_storage->tokens[i]);
    }
    printf("Length of token storage -> %d\n", token_storage->num_tokens);

    token_storage_free(token_storage);
    return error;
}
