#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "stack.h"
#include "tokenizer.h"

bool rule_expr(token_storage_t *token_storage) {
    (void) token_storage;
    get_token(token_storage);
    return 1;
}