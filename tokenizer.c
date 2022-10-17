#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

const char *TOK_STR[] = {"TOK_ID", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL"};
const char whitespace = ' ';

typedef enum {STATE_START, STATE_ID_START, STATE_ID_MAIN, STATE_ID} state;

// constructor for struct Token
token_t *token_create(TOK_TYPE token_type_, const char *start_ptr, int lex_length) {
    token_t *token = malloc(sizeof(token_t));
    // check if malloc failed
    if (token == NULL) {
        // ERROR(1)
        return NULL;
    }
    token->token_type = token_type_;

    // create memory for lexeme
    token->value = malloc(lex_length * sizeof(char)); 
    for (int i = 0; i < lex_length; i++) {
        token->value[i] = start_ptr[i];     
    }

    return token;
}

// destructor for struct Token
void token_free(token_t *token) {
    free(token->value);
    token->value = NULL;
    free(token);
}

// function for debugging purposes
void token_print(token_t *token) {
    if (token == NULL) {
        return;
    }
    printf("Token->type: %s\nToken->value: %s\n", TOK_STR[token->token_type], token->value);
}

// deterministic finite automata
// returns 1 if there was an error during tokenization
int dka(const char *source, token_t *tokens) {
    return 0;
}

// typedef enum {STATE_START, STATE_ID_START, STATE_ID_MAIN, STATE_ID} state;


