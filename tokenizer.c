#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

const char *TOK_STR[] = {"TOK_ID", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL"};
const char whitespace = ' ';


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
int dka(char *source, int source_len, token_t *tokens) {
    // we'll iterate through all the characters in source
    // each function will return a state
    // the returned state will determine the next function to be called
    state current_state = STATE_START;
    char *start_ptr = source;
    int token_value_len = 0;
    for (int i = 0; i < source_len; i++) {
        switch (current_state) {
            case STATE_START:
                current_state = function_start(*(start_ptr + token_value_len));
                token_value_len++;
                return 0;
            case STATE_ID:
                token_t *new_token = token_create(TOK_ID, start_ptr, token_value_len);
                start_ptr += token_value_len;
                (void) new_token;
                (void) start_ptr;
            default:
                return 0;

                // pridam tam token
        }
        source += 1;
    }
    return 0;
}

// typedef enum {STATE_START, STATE_ID_START, STATE_ID_MAIN, STATE_ID} state;

state function_start(char c){
    if (c == whitespace){
        printf("Tentok more gadzo je whitespace\n");
        return STATE_START; 
    }
    else if (c == '$'){
        printf("Tentok more gadzo je $\n");
        return STATE_ID_START;
    }
    printf("Je to volaco ine\n");
    return STATE_ERROR;
}

