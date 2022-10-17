#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {TOK_ID, TOK_KEYWORD, TOK_SEPARATOR, TOK_OPERATOR, TOK_LITERAL} TOK_TYPE;

typedef struct Token {
    TOK_TYPE token_type;
    char *value;
} token_t;

token_t *token_create(TOK_TYPE token_type_, const char *start_ptr, int lex_length);
void token_free(token_t *token);
void token_print(token_t *token);

#endif
