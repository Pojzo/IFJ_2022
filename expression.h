#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tokenizer.h"
#include "list.h"

bool rule_expr(token_storage_t *token_storage);
bool rule_expr2(list_t** list, token_storage_t* token_storage);
int main_alg(list_t **list, symbol_enum top, symbol_enum input, token_storage_t* token_storage, bool input_ended);
void eq_assoc(list_t **list, symbol_enum input);
void right_assoc(list_t **list, symbol_enum input);
bool left_assoc(list_t **list);
bool rule_check(list_t **list);
bool check_end(token_t* token, int* left_brackets);
symbol_enum convert_token_to_symbol(token_t *token, bool* valid);
int convert_symbol_to_int(symbol_enum symbol);

#endif
