/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tokenizer.h"
#include "list.h"
#include "symtable.h"

bool rule_expr(token_storage_t *token_storage, datatype_t *datatype, bool can_null);
bool rule_expr2(list_t** list, token_storage_t* token_storage, bool is_string);
int main_alg(list_t **list, symbol_enum top, symbol_enum input, token_storage_t* token_storage, bool input_ended, bool is_string);
datatype_t convert_symbol_to_datatype(symbol_enum symbol);
void eq_assoc(list_t **list, symbol_enum input);
void right_assoc(list_t **list, symbol_enum input);
bool left_assoc(list_t **list, bool is_string);
bool rule_check(list_t **list, bool is_string);
bool check_end(token_t* token, int* left_brackets);
symbol_enum convert_token_to_symbol(token_t *token, bool* valid);
int convert_symbol_to_int(symbol_enum symbol);
bool contains_dot(char *);
bool compatible_operands(symbol_enum op , bool is_string);
bool term_idfun_call_alt(token_storage_t *token_storage);
bool term_open_bracket_alt(token_storage_t *token_storage);
bool term_close_bracket_alt(token_storage_t *token_storage);
bool rule_funccallarg_alt(token_storage_t *token_storage, char *function_name);
bool rule_next_expr_alt(token_storage_t *token_storage, int anticipated_args, int curr_args, id_node_t *function_name);
bool term_comma_alt(token_storage_t *token_storage);
#endif
