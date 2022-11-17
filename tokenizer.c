#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "utils.h"

const int DEBUG = 0;

const char *TOK_STR[] = {"TOK_ID", "TOK_ID_FUNCTION", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL"};
const char whitespace = ' ';
const char operators[] = {'*', '+', '-', '=', '/', '%', '<', '>', '.'};
const char *string_operators[] = {"===", "!==", ">=", "<="};
const char *keywords[] = {"if", "else", "float", "function", "int", "null", "return", "string", "while", "void"}; 
const char separators[] = {' ', '(', ')', '{', '}', '[', ']', ';', ',', '\n', '\r'};
token_t *tokens;
const int separ_len = 11;
const int oper_len = 9;
const int string_oper_len = 5;
const int keywords_len = 10;

int buffer_read_len = 0;


//fucntion checks whether string is keyword 
bool is_keyword(char *start_ptr, int token_value_len){
    char *curr_str = malloc((token_value_len + 1) * sizeof(char));
    for (int i = 0; i < token_value_len; i++){
        curr_str[i] = (char)start_ptr[i]; 
    }
    curr_str[token_value_len] = '\0'; 
    if (arr_contains_str(keywords, curr_str, keywords_len)){
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
    state current_state = STATE_START;
    char *start_ptr = source;
    int token_value_len = 0;
    int i = 0;
    while (i <= source_len) {
        char current_char = source[i];
        switch (current_state) {
            case STATE_START:
                if (current_char == ' ' || current_char == '\n') {
                    start_ptr++;
                    i++;
                    current_state = STATE_START;
                    break;
                }
                if (DEBUG) debug_print_state("STATE_START", start_ptr, 1);
                printf("%c %d\n", current_char, current_char == *start_ptr);
                token_value_len = 1;
                i++;
                current_state = state_start(current_char);
                break;

            case STATE_ID_START:
                if (DEBUG) debug_print_state("STATE_ID_START", start_ptr, token_value_len);
                current_state = state_id_start(current_char);
                token_value_len++;
                i++;
                break;

            case STATE_ID_MAIN:
                if (DEBUG) debug_print_state("STATE_ID_MAIN", start_ptr, token_value_len);
                current_state = state_id_main(current_char);
                // we read the whole identifier
                if (current_state == STATE_ID) {
                    // printf("Current char: %c %d\n", current_char, current_char == '\n');
                    token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                    start_ptr += token_value_len;
                    current_state = STATE_START;
                    break;
                }
                token_value_len++;
                i++;
                break;

            case STATE_KEYWORD_MAIN:
                if (DEBUG) debug_print_state("STATE_KEYWORD_MAIN", start_ptr, token_value_len);
                current_state = state_keyword_main(current_char);
                if (current_state == STATE_KEYWORD) {
                    if (is_keyword(start_ptr, token_value_len)) {
                        token_storage_add(token_storage, TOK_KEYWORD, start_ptr, token_value_len);
                    }
                    else {
                        token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                    }
                    start_ptr += token_value_len;
                    current_state = STATE_START;
                    break;
                }
                token_value_len++;
                i++;
                break;

            case STATE_SEP:
                if (DEBUG) debug_print_state("STATE_SEP", start_ptr, token_value_len);
                current_state = STATE_START;
                token_storage_add(token_storage, TOK_SEPARATOR, start_ptr, token_value_len);
                start_ptr++;
                break;

            // /...
            case STATE_COMMENT_START:
                if (DEBUG) debug_print_state("STATE_COMMENT_START", start_ptr, token_value_len);
                if (current_char == '/') {
                    token_value_len++;
                    current_state = STATE_COMMENT_SINGLE; 
                    i += 1;
                }
                else if (current_char == '*') {
                    token_value_len++;
                    current_state = STATE_COMMENT_MULTI;
                    i += 1;
                }
                else {
                    // if it's not a comment then it must be divisor operator
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, 1);
                    start_ptr += 1;
                    current_state = STATE_START;
                }

                break;

            //.....
            case STATE_COMMENT_SINGLE:
                if (DEBUG) debug_print_state("STATE_COMMENT_SINGLE", start_ptr, token_value_len);
                // only end comment with new line
                if (current_char == '\n') {
                    start_ptr += token_value_len + 1;
                    i++;
                    current_state = STATE_START;
                }
                else {
                    token_value_len++;
                    i++;
                    current_state = STATE_COMMENT_SINGLE;
                }
                break;

            // /*....
            case STATE_COMMENT_MULTI:
                if (DEBUG) debug_print_state("STATE_COMMENT_MULTI", start_ptr, token_value_len);
                if (current_char == '*') {
                    current_state = STATE_COMMENT_MULTI2;
                    i++;
                    token_value_len++;
                }
                else {
                    current_state = STATE_COMMENT_MULTI;
                    i++;
                    token_value_len++;
                }
                break;

            // /*... *
            case STATE_COMMENT_MULTI2:
                if (DEBUG) debug_print_state("STATE_COMMENT_MULTI2", start_ptr, token_value_len);
                if (current_char == '/') {
                    start_ptr += token_value_len + 1;
                    current_state = STATE_START;
                    i += 1;
                }
                // if its not the go back to /*...
                else {
                    current_state = STATE_COMMENT_MULTI;
                    i += 1;
                }
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
        return STATE_KEYWORD_MAIN;
    }
    else if (arr_contains_char(separators, c, separ_len)) {
        return STATE_SEP;
    }
    else if (c == '/') {
        return STATE_COMMENT_START;
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

state state_keyword_main(char c) {
    if (is_alpha(c) || is_digit(c) || c == '_') {
        return STATE_KEYWORD_MAIN;
    }
    if (arr_contains_char(separators, c, separ_len) || arr_contains_char(operators, c, oper_len)){
        return STATE_KEYWORD; 
    }
    return STATE_ERROR;
}
