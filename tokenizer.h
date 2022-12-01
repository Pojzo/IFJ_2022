#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>


typedef enum {TOK_ID, TOK_ID_FUNCTION, TOK_KEYWORD, TOK_SEPARATOR, TOK_OPERATOR, TOK_LIT, TOK_EPILOG, TOK_SPECIAL} tok_type;

typedef enum {STATE_START, 
	STATE_ID_START, STATE_ID_MAIN, STATE_ID,
       	STATE_KEYWORD_START, STATE_IS_KEYWORD, STATE_KEYWORD_MAIN, STATE_KEYWORD,
       	STATE_SEP,
       	STATE_LIT_NUM, STATE_LIT_STR,
		STATE_OP,
       	STATE_ERROR,
		STATE_EPILOG,
		STATE_EPILOG_2,
STATE_COMMENT_START, STATE_COMMENT_SINGLE, STATE_COMMENT_MULTI, STATE_COMMENT_MULTI2,
STATE_EQUAL_1, STATE_EQUAL_2, STATE_GREATER_EQUAL, STATE_SMALLER_EQUAL, STATE_NOT_1, STATE_NOT_2, STATE_LIT_NUM_FLOAT, STATE_LIT_NUM_FLOAT_E, STATE_LIT_NUM_FLOAT_E_2, STATE_LIT_NUM_FLOAT_E_3, STATE_TERMINATE} state;

typedef struct Token {
    tok_type token_type;
    char *value;
} token_t;

typedef struct Token_Storage {
    token_t **tokens;
    int num_tokens;
    int array_len;
} token_storage_t;

token_t *token_create(tok_type token_type_, const char *start_ptr, int lex_length);
void token_free(token_t *token);
void token_print(token_t *token);

token_storage_t *token_storage_create();
void token_storage_free(token_storage_t *token_storage);
void token_storage_add(token_storage_t *token_storage, tok_type token_type, char *start_ptr, int token_value_len);

bool is_keyword(char *start_ptr, int token_value_len);
int dka(char *source, int source_len, token_storage_t *token_storage);

// we'll put all states here
state state_start(char c);

// identifier states
state state_id_start(char c);
state state_id_main(char c);

// keyword states
state state_keyword_start(char c);
state state_keyword_main(char c);
state state_is_keyword(char *start_ptr, int token_value_len);

token_t *get_token(token_storage_t *token_storage);
token_t *get_token_keep(token_storage_t *token_storage);



#endif
