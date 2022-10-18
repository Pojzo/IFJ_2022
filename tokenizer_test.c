#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

int main() {
    char *buffer = "gazdik";
    int buffer_len = strlen(buffer);
    token_t *tokens = NULL;
    dka(buffer, buffer_len, tokens);
    return 0;
}
