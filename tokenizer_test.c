#include <stdio.h>
#include "tokenizer.h"

int main() {
    char *buffer = "abcdefghij";
    token_t *token = token_create(TOK_ID, buffer, 3);
    token_print(token);

    token_t *token2 = token_create(TOK_KEYWORD, buffer + sizeof(char) * 2, 3);
    token_print(token2);

    return 0;
}
