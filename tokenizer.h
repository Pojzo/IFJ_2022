#ifndef TOKENIZER_H
#define TOKENIZER_H


typedef enum {TOK_ID, TOK_KEYWORD, TOK_SEPARATOR, TOK_OPERATOR, TOK_LITERAL} tok_type;

typedef enum {STATE_START, STATE_ID_START, STATE_ID_MAIN, STATE_ID, STATE_KEYWORD_START, STATE_KEYWORD_MAIN, STATE_KEYWORD, STATE_ERROR} state;

typedef struct Token {
    tok_type token_type;
    char *value;
} token_t;

typedef struct Token_Storage {
    token_t **tokens;
    int num_tokens;
    int array_len;
} token_array_t;

token_t *token_create(tok_type token_type_, const char *start_ptr, int lex_length);
void token_free(token_t *token);
void token_print(token_t *token);

token_array_t *token_array_create();
void token_array_free(token_array_t *token_array);
void token_array_add(token_array_t *token_array, tok_type token_type, char *start_ptr, int token_value_len);

int dka(char *source, int source_len, token_t *tokens);


// we'll put all states here
state state_start(char c);

// identifier states
state state_id_start(char c);
state state_id_main(char c);

// keyword states
state state_keyword_start(char c);
state state_keyword_main(char c);

#endif
