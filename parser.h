#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"


int parser_start(char *buffer);
token_t *get_token();


#endif