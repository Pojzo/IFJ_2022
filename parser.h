#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <stdbool.h>
#include "symtable.h"



int parser_start(char *buffer);
int check_strict_types(token_storage_t *token_storage);
bool check_prolog(char *buffer);
bool rule_program(token_storage_t *token_storage);
bool rule_st(token_storage_t *token_storage);
bool rule_stlist(token_storage_t *token_storage);
bool rule_fstlist(token_storage_t *token_storage);
bool rule_else(token_storage_t *token_storage);
bool rule_funccallarg(token_storage_t *token_storage);
bool rule_next(token_storage_t *token_storage);
bool term_if(token_storage_t *token_storage);
bool term_while(token_storage_t *token_storage);
bool term_open_bracket(token_storage_t *token_storage);
bool term_close_bracket(token_storage_t *token_storage);
bool term_open_curly_bracket(token_storage_t *token_storage);
bool term_close_curly_bracket(token_storage_t *token_storage);
bool term_else(token_storage_t *token_storage);
bool term_function(token_storage_t *token_storage);
bool term_equals(token_storage_t *token_storage);
bool term_semicolon(token_storage_t *token_storage);
bool term_colon(token_storage_t *token_storage);
bool term_comma(token_storage_t *token_storage);
bool term_id(token_storage_t *token_storage);
bool term_idfun(token_storage_t *token_storage, bool is_fdef);
bool term_type(token_storage_t *token_storage);
bool rule_fdef (token_storage_t *token_storage);
bool rule_function_arguments(token_storage_t *token_storage);
bool rule_argf(token_storage_t *token_storage);
bool rule_more_argf(token_storage_t *token_storage);
bool rule_function_body(token_storage_t* token_storage);
bool rule_cond(token_storage_t* token_storage);
bool rule_cond_end(token_storage_t* token_storage);
bool term_cond_op(token_storage_t *token_storage);
bool rule_return_cond(token_storage_t* token_storage);
bool rule_return_cond_end(token_storage_t* token_storage);
bool term_idfun_call(token_storage_t *token_storage);




#endif
