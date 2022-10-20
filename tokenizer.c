#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

const char *TOK_STR[] = {"TOK_ID", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL"};
const char whitespace = ' ';

token_t *tokens;

// constructor for struct Token
token_t *token_create(tok_type token_type_, const char *start_ptr, int lex_length) {
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
    (void) tokens;
    /*
    const char *test = "12345";
    token_t *token1 = token_create(TOK_ID, test, 1);
    token_t *token2 = token_create(TOK_KEYWORD, test, 2);
    token_t *token3 = token_create(TOK_SEPARATOR, test, 3);
    token_t *token4 = token_create(TOK_LITERAL, test, 4);
    */
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
                (void) token_value_len;
                return 0;
            case STATE_ID:
                // token_t *new_token = token_create(TOK_ID, start_ptr, token_value_len);
                start_ptr += token_value_len;
                // (void) new_token;
                (void) start_ptr;
                return 0;
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

// ADDED
// constructor for token_array
token_array_t *token_array_create() {
    // dynamically allocate space for token_array structure
    token_array_t *token_array = malloc(sizeof(token_array_t));

    // check if malloc failed
    if (token_array == NULL) {
        // TODO tuto dame nejaky error;
        return NULL;
    }

    token_array->tokens = malloc(0);

    // num tokens represents the number of tokens currently present in the array
    token_array->num_tokens = 0;

    // array_len represents memory allocated for array_len tokens
    token_array->array_len = 1;

    return token_array;
}

// ADDED
void token_array_free(token_array_t *token_array) {
    // iterate over tokens and free each one of them
    for (int i = 0; i < token_array->array_len; i++) {
        token_free(token_array->tokens[i]);
        token_array->tokens[i] = NULL;
    }
    free(token_array);
}

// ADDED
// adds token to token array
void token_array_add(token_array_t *token_array, tok_type token_type, char *start_ptr, int token_value_len) {
    // create new token
    token_t *token = token_create(token_type, start_ptr, token_value_len);
    if (token == NULL) {
        // TODO tu dame nejaky error
    }
    
    // if num_tokens == array_len - if number of tokens is equal to the number of allocated space, we should inflate the
    // array
    
    if (token_array->num_tokens == token_array->array_len) {
        token_array->array_len *= 2;
        token_array->tokens = realloc(token_array->tokens, sizeof(token_t) * token_array->array_len);
    }
    // add the newly created token to our array
    token_array->tokens[token_array->num_tokens++] = token;
}
