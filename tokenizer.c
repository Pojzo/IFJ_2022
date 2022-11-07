#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "utils.h"

// nerd face

const char *TOK_STR[] = {"TOK_ID", "TOK_ID_FUNCTION", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL"};
const char whitespace = ' ';
const char operators[] = {'*', '+', '-', '=', '/', '%', '<', '>', '.'};
const char *keywords[] = {"if", "else", "float", "function", "int", "null", "return", "string", "while", "void"}; 
const char separators[] = {' ', '(', ')', '{', '}', '[', ']', ';', ',', '\n', '\r'};
token_t *tokens;
const int separ_len = 11;
const int oper_len = 9;
const int keywords_len = 10;

int buffer_read_len = 0;


//fucntion checks whether string is keyword 
bool is_keyword(char *start_ptr, int token_value_len){
    char *curr_str = malloc((token_value_len + 1) * sizeof(char));
    for (int i = 0; i <= token_value_len; i++){
        curr_str[i] = (char)start_ptr[i]; 
    }
    curr_str[token_value_len] = '\0'; 
    if (arr_contains_str(keywords, curr_str, token_value_len)){
        return true;
    }
    return false;
}


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
    token->value = malloc((lex_length + 1) * sizeof(char)); 
    for (int i = 0; i < lex_length; i++) {
        token->value[i] = (char)start_ptr[i];     
    }
    token->value[lex_length] = '\0';

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

// ADDED
// constructor for token_storage
token_storage_t *token_storage_create() {
    // dynamically allocate space for token_storage structure
    token_storage_t *token_storage = malloc(sizeof(token_storage_t));

    // check if malloc failed
    if (token_storage == NULL) {
        // TODO tuto dame nejaky error;
        return NULL;
    }

    token_storage->tokens = NULL;

    // num tokens represents the number of tokens currently present in the array
    token_storage->num_tokens = 0;

    // array_len represents memory allocated for array_len tokens
    token_storage->array_len = 1;

    return token_storage;
}

// ADDED
void token_storage_free(token_storage_t *token_storage) {
    // iterate over tokens and free each one of them
    for (int i = 0; i < token_storage->num_tokens; i++) {
        token_free(token_storage->tokens[i]);
        token_storage->tokens[i] = NULL;
    }
    free(token_storage->tokens);
    free(token_storage);
}

// ADDED
// adds token to token array
void token_storage_add(token_storage_t *token_storage, tok_type token_type, char *start_ptr, int token_value_len) {
    // create new token
    token_t *token = token_create(token_type, start_ptr, token_value_len);
    if (token == NULL) {
        // TODO tu dame nejaky error
    }

    // if num_tokens == array_len - if number of tokens is equal to the number of allocated space, we should inflate the
    // array

    if (token_storage->num_tokens + 1 == token_storage->array_len) {
        // increase the size by 2 to avoid too many reallocs
        token_storage->array_len *= 2;
        token_storage->tokens = realloc(token_storage->tokens, sizeof(token_t) * token_storage->array_len);
    }
    // add the newly created token to our array
    token_storage->tokens[token_storage->num_tokens++] = token;
}

// deterministic finite automata
// returns 1 if there was an error during tokenization
int dka(char *source, int source_len, token_storage_t *token_storage) {
    (void) source_len;
    state current_state = STATE_START;
    char *start_ptr = source;
    int token_value_len = 0;
    int i = 0;
    while (i <= source_len) {
        switch (current_state) {
            case STATE_START:
                if (*(start_ptr + token_value_len) == ' ') {
                    start_ptr += 1;
                    token_value_len = 0;
                    i++;
                    continue;
                }
                current_state = state_start(*(start_ptr + token_value_len));
                token_value_len++;
                i++;
                break;

            case STATE_ID_START:
                current_state = state_id_start(*(start_ptr + token_value_len));
                token_value_len++;
                i++;
                break;

            case STATE_ID_MAIN:
                current_state = state_id_main(*(start_ptr + token_value_len));
                if (current_state == STATE_ID) {
                    token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                    start_ptr += token_value_len;
                    current_state = STATE_START;
                    token_value_len = 0;
                    break;
                }
                token_value_len++;
                i++;
                break;

            case STATE_KEYWORD_MAIN:
                current_state = state_keyword_main(*(start_ptr + token_value_len));
                if (current_state == STATE_KEYWORD) {
                    if (is_keyword(start_ptr, token_value_len)) {
                        token_storage_add(token_storage, TOK_KEYWORD, start_ptr, token_value_len);
                    }
                    else {
                        token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                    }
                    start_ptr += token_value_len;
                    current_state = STATE_START;
                    token_value_len = 0;
                    break;
                }
                token_value_len++;
                i++;
                break;
/*
            case STATE_ID:
                current_state = STATE_START;
                token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                start_ptr += token_value_len;
                token_value_len = 0;
                //start_ptr--;
                break;
*/

            case STATE_IS_KEYWORD:
                state_is_keyword(start_ptr + 1, token_value_len -1);
                break;

            case STATE_SEP:
                current_state = STATE_START;
                token_storage_add(token_storage, TOK_SEPARATOR, start_ptr, token_value_len);
                start_ptr++;
                token_value_len = 0;
                break;
                
            case STATE_ERROR:
                return 1;
            default:
                break;
        }
    }

    return 0;
}


// start state
state state_start(char c){
    if (c == whitespace || c == '\n'){
        return STATE_START; 
    }
    else if (c == '$'){
        return STATE_ID_START;
    }
    else if (is_lower(c)) {
        return STATE_KEYWORD_START;
    }
    else if (arr_contains_char(separators, c, separ_len)) {
        return STATE_SEP;
    }
    else if (c == '/') {
        return STATE_OP_START;
    }
    else if (arr_contains_char(operators, c, oper_len)) {
        return STATE_OP;
    }
    else if (is_digit(c)) {
        return STATE_LIT_NUM;
    }
    else if (c == '"') {
        return STATE_LIT_STR;
    }

    return STATE_ERROR;
}

// identifier states
state state_id_start(char c) {
    if (is_alpha(c) || c ==  '_') { 
        return STATE_ID_MAIN; 
    }
    return STATE_ERROR; 
}

state state_id_main(char c) {
    if (is_alpha(c) || is_digit(c) || c == '_') {
        return STATE_ID_MAIN;
    }
    if (arr_contains_char(separators, c, separ_len) || arr_contains_char(operators, c, oper_len)){
        return STATE_ID; 
    }
    return STATE_ERROR;
}
//nerdy face
state state_keyword_main(char c) {
    if (is_alpha(c) || is_digit(c) || c == '_') {
        return STATE_KEYWORD_MAIN;
    }
    if (arr_contains_char(separators, c, separ_len) || arr_contains_char(operators, c, oper_len)){
        return STATE_KEYWORD; 
    }
    return STATE_ERROR;
}
//TODO sem musime doniest vsetky tie nacitane pismenka a chujoviny 
state state_is_keyword(char *start_ptr, int token_value_len){
    if (is_keyword(start_ptr, token_value_len)) {
        return STATE_ERROR;
    }
    return STATE_ID;

    // keyword states
    /*state state_keyword_start(char c) {
    // POBO (TODO)
    (void) c;
    return STATE_KEYWORD;
    }
    */
}
