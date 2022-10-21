#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

int main() {
    char *buffer = "gazdik";
    token_array_t *token_array = token_array_create();
    token_array_add(token_array, TOK_ID, buffer, 2);
    token_array_add(token_array, TOK_KEYWORD, buffer, 3);
    token_array_add(token_array, TOK_SEPARATOR, buffer, 4);
    token_array_add(token_array, TOK_LITERAL, buffer, 5);
    for(int i = 0; i < 4; i++) {
        token_print(token_array->tokens[i]);
    }
    printf("Tokenizer->num_tokens: %d\nTokenizer->array_len %d\n", token_array->num_tokens, token_array->array_len);
    token_array_free(token_array);
    // dka(buffer, buffer_len, tokens);
    return 0;
}
