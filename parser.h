#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <stdbool.h>


int parser_start(char *buffer);
token_t *get_token(token_storage_t *token_storage);
int check_strict_types(token_storage_t *token_storage);
bool check_prolog(char *buffer);
bool rule_program(token_storage_t *token_storage);
bool rule_st(token_storage_t *token_storage);
bool rule_st_list(token_storage_t *token_storage);


#endif