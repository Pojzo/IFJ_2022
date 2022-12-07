/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#include <stdio.h>
#include <stdlib.h>


#include "tokenizer.h"
#include "utils.h"
#include <assert.h>
#include <string.h>

extern const int DEBUG_LEXER;

const char *TOK_STR[] = {"TOK_ID", "TOK_ID_FUNCTION", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL", "TOK_EPILOG"};
const char whitespace = ' ';
const char operators[] = {'*', '+', '-', '=', '/', '%', '<', '>', '.'};
const char *string_operators[] = {"===", "!==", ">=", "<="};
const char *keywords[] = {"if", "else", "float", "?float", "function", "int", "?int",
                          "null", "return", "string", "?string", "while", "void", "boolean"};
const char separators[] = {' ', ':', '(', ')', '{', '}', '[', ']', ';', ',', '\n', '\r'};
token_t *tokens; 
const int separ_len = 12;
const int oper_len = 9;
const int string_oper_len = 4;
const int keywords_len = 14;

int buffer_read_len = 0;
extern const char *epilog;
extern int token_index;

extern const int DEBUG_PARSER;


bool terminated_string = false;



//fucntion checks whether string is keyword 
bool is_keyword(char *start_ptr, int token_value_len){
    char *curr_str = malloc((token_value_len + 1) * sizeof(char));
    for (int i = 0; i < token_value_len; i++){
        curr_str[i] = (char)start_ptr[i]; 
    }
    curr_str[token_value_len] = '\0'; 
    if (arr_contains_str(keywords, curr_str, keywords_len)){
        free(curr_str);
        return true;
    }
    free(curr_str);
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
    printf("Token->type: %s\nToken->value: %s - %d\n", TOK_STR[token->token_type], token->value, (int)strlen(token->value));
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
    token_t *token = token_create(token_type, start_ptr, token_value_len);
    if (token == NULL) {
        // TODO tu dame nejaky error
    }

    // if there is no space in the array, reallocate it
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
int token_value_len = 0;
int i = 0;
char *start_ptr;
int dka(char *source, int source_len, token_storage_t *token_storage) {
    state current_state = STATE_START;
    start_ptr = source;
    while (i < source_len) {
        char current_char = source[i];
        switch (current_state) {
            case STATE_START:
                if (current_char == ' ' || current_char == '\n') {  
                    start_ptr++;
                    i++;
                    current_state = STATE_START;
                    break;
                }
                if (DEBUG_LEXER) debug_print_state("STATE_START", start_ptr, 1);
                start_ptr = &source[i];
                token_value_len = 1;
                current_state = state_start(current_char);
                break;


            case STATE_ID_START:
                if (DEBUG_LEXER) debug_print_state("STATE_ID_START", start_ptr, token_value_len);
                current_state = state_id_start(current_char);
                token_value_len++;
                i++;
                break;

            case STATE_ID_MAIN:
                if (DEBUG_LEXER) debug_print_state("STATE_ID_MAIN", start_ptr, token_value_len);
                current_state = state_id_main(current_char);
                // we read the whole identifier
                if (current_state == STATE_ID) {
                    // printf("Current char: %c %d\n", current_char, current_char == '\n');
                    token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                    current_state = STATE_START;
                    break;
                }
                token_value_len++;
                i++;
                break;

            case STATE_KEYWORD_MAIN:
                if (DEBUG_LEXER) debug_print_state("STATE_KEYWORD_MAIN", start_ptr, token_value_len);
                current_state = state_keyword_main(current_char);
                if (current_state == STATE_KEYWORD) {
                    if (is_keyword(start_ptr, token_value_len)) {
                        token_storage_add(token_storage, TOK_KEYWORD, start_ptr, token_value_len);
                    }
                    else {
                        token_storage_add(token_storage, TOK_ID, start_ptr, token_value_len);
                    }
                    current_state = STATE_START;
                    break;
                }
                token_value_len++;
                i++;
                break;

            case STATE_SEP:
                if (DEBUG_LEXER) debug_print_state("STATE_SEP", start_ptr, token_value_len);
                current_state = STATE_START;
                token_storage_add(token_storage, TOK_SEPARATOR, start_ptr, token_value_len);
                break;

            // /...
            case STATE_COMMENT_START:
                if (DEBUG_LEXER) debug_print_state("STATE_COMMENT_START", start_ptr, token_value_len);
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
                    current_state = STATE_START;
                }

                break;

            //.....
            case STATE_COMMENT_SINGLE:
                if (DEBUG_LEXER) debug_print_state("STATE_COMMENT_SINGLE", start_ptr, token_value_len);
                // only end comment with new line
                if (current_char == '\n') {
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
                if (DEBUG_LEXER) debug_print_state("STATE_COMMENT_MULTI", start_ptr, token_value_len);
                if(i == source_len -1){
                    current_state = STATE_ERROR;
                }
                else if (current_char == '*') {
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
                if (DEBUG_LEXER) debug_print_state("STATE_COMMENT_MULTI2", start_ptr, token_value_len);
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
            
            case STATE_OP:
                if (DEBUG_LEXER) debug_print_state("STATE_OPERATOR", start_ptr, 1);
                current_state = STATE_START;
                token_storage_add(token_storage, TOK_OPERATOR, start_ptr, 1);
                i++;
                break;
                
            case STATE_EQUAL_1:
                // =...
                // token_value_len = 2;
                if (DEBUG_LEXER) debug_print_state("STATE_EQUAL_1", start_ptr, token_value_len);
                if (current_char == '=') {
                    token_value_len++;
                    current_state = STATE_EQUAL_2;
                    i++;
                }
                else {
                    current_state = STATE_START;
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, 1);
                }
                break;
            case STATE_EQUAL_2:
            //  ==...
                if (DEBUG_LEXER) debug_print_state("STATE_EQUAL_2", start_ptr, token_value_len);
                if (current_char == '=') {
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, token_value_len);
                    current_state = STATE_START;
                    i++;
                }
                else {
                    current_state = STATE_ERROR;
                }
                break;
            
            case STATE_NOT_1:
            // !...
                if (DEBUG_LEXER) debug_print_state("STATE_NOT_1", start_ptr, token_value_len);
                if (current_char == '=') {
                    token_value_len++;
                    current_state = STATE_NOT_2;
                    i++;
                }
                else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_NOT_2:
            // !=...
                if (DEBUG_LEXER) debug_print_state("STATE_NOT_2", start_ptr, token_value_len);
                if (current_char == '=') {
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, token_value_len);
                    current_state = STATE_START;
                    i++;
                }
                else {
                    current_state = STATE_ERROR;
                }
                break;
            
            case STATE_GREATER_EQUAL:
            // >...
                if (DEBUG_LEXER) debug_print_state("STATE_GREATER_EQUAL", start_ptr, token_value_len);
                if (current_char == '=') {
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, token_value_len);
                    current_state = STATE_START;
                    i++;
                }
                else {
                    
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, 1);
                    current_state = STATE_START;
                }
                break;
            
            case STATE_SMALLER_EQUAL:
            // <...
                if (DEBUG_LEXER) debug_print_state("STATE_SMALLER_EQUAL", start_ptr, token_value_len);
                if (current_char == '=') {
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, token_value_len);
                    current_state = STATE_START;
                    i++;
                }
                else {
                    
                    token_storage_add(token_storage, TOK_OPERATOR, start_ptr, 1);
                    current_state = STATE_START;
                }
                break;
            
            case STATE_LIT_NUM:
            // 2)
            // 123...
                if (DEBUG_LEXER) debug_print_state("STATE_LIT_NUM", start_ptr, token_value_len);
                if (is_digit(current_char)) {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_NUM;
                }
                else if (current_char == 'e' || current_char == 'E') {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_NUM_FLOAT_E;
                }
                else if (current_char == '.' /*&& is_digit(source[i+1])*/) {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_NUM_FLOAT;
                }
                else {
                    token_storage_add(token_storage, TOK_LIT, start_ptr, token_value_len);
                    current_state = STATE_START;
                }
                break;

            case STATE_LIT_NUM_FLOAT_E:
                // 1e..
                if (DEBUG_LEXER) debug_print_state("STATE_LIT_NUM_FLOAT_E", start_ptr, token_value_len);
                if (current_char == '+' || current_char == '-') {
                    token_value_len++;
                    i++;
                }
                current_state = STATE_LIT_NUM_FLOAT_E_2;
                break;

            case STATE_LIT_NUM_FLOAT_E_2:
                if (is_digit(current_char)) {
                    i++;
                    token_value_len++;
                    current_state = STATE_LIT_NUM_FLOAT_E_3;
                }
                else {
                    current_state = STATE_ERROR;
                }
                break;
            case STATE_LIT_NUM_FLOAT_E_3:
                // 1e(+-)..
                if (DEBUG_LEXER) debug_print_state("STATE_LIT_NUM_FLOAT_E_2", start_ptr, token_value_len);
                if (is_digit(current_char)) {
                    printf("tu sme boli\n");
                    i++;
                    token_value_len++;
                    current_state = STATE_LIT_NUM_FLOAT_E_3;
                }
                else {
                    token_storage_add(token_storage, TOK_LIT, start_ptr, token_value_len);
                    current_state = STATE_START;
                }
                break;

            case STATE_LIT_NUM_FLOAT:
            // 123. ...
                if (DEBUG_LEXER) debug_print_state("STATE_LIT_NUM_FLOAT", start_ptr, token_value_len);
                if (is_digit(current_char)) {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_NUM_FLOAT_2;
                }
                else {
                    current_state = STATE_ERROR;

                }
                break;

            case STATE_LIT_NUM_FLOAT_2:
                if (is_digit(current_char)) {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_NUM_FLOAT_2;
                }
                else if (current_char == 'e' || current_char == 'E') {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_NUM_FLOAT_E;
                }
                else{
                    token_storage_add(token_storage, TOK_LIT, start_ptr, token_value_len);
                    current_state = STATE_START;
                }
                break;

            
            case STATE_LIT_STR:
            // "..
                if (i + 1 == source_len) {
                    if (current_char != '"') {
                        return 1;
                    }
                }

                if (DEBUG_LEXER) debug_print_state("STATE_LIT_STR", start_ptr, token_value_len);
                if (current_char == '$' && source[i-1] != '\\'){
                    current_state = STATE_ERROR;
                }
                else if ((current_char == '"' && source[i-1] != '\\') || (current_char == '"' && source[i-1] == '\\' && source[i-2] == '\\')) {
                    token_storage_add(token_storage, TOK_LIT, start_ptr, token_value_len);
                    current_state = STATE_START;
                    i++;
                }
                else {
                    token_value_len++;
                    i++;
                    current_state = STATE_LIT_STR;
                }
                break;
            
            case STATE_EPILOG:
            // ?..
                if (current_char == '>') {
                    token_storage_add(token_storage, TOK_EPILOG, start_ptr, token_value_len);
                    current_state = STATE_EPILOG_2;
                    i++;
                }
                else if(current_char == 'i'){
                    current_state = STATE_QUESTION_INT;
                    token_value_len++;
                    i++;
                }
                else if(current_char == 'f'){
                    current_state = STATE_QUESTION_FLOAT;
                    token_value_len++;
                    i++;
                }
                else if(current_char == 's'){
                    current_state = STATE_QUESTION_STRING;
                    token_value_len++;
                    i++;
                }
                else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_QUESTION_INT:
                if(current_char == 'n'){
                    i++;
                    token_value_len++;
                    current_char = source[i];
                    if(current_char == 't'){
                        token_storage_add(token_storage, TOK_KEYWORD, start_ptr, token_value_len);
                        i++;
                        current_state = STATE_START;
                    }
                    else{
                        return 1;
                    }
                }
                break;
            case STATE_QUESTION_FLOAT:
                if(current_char == 'l'){
                    i++;
                    token_value_len++;
                    current_char = source[i];
                    if(current_char == 'o'){
                        i++;
                        token_value_len++;
                        current_char = source[i];
                        if(current_char == 'a'){
                            i++;
                            token_value_len++;
                            current_char = source[i];
                        }
                        if(current_char == 't'){
                            token_storage_add(token_storage, TOK_KEYWORD, start_ptr, token_value_len);
                            i++;
                            current_state = STATE_START;
                        }
                    }
                }
                else {
                    return 1;
                }
                break;

            case STATE_QUESTION_STRING:
                if(current_char == 't'){
                    printf("tu\n");
                    i++;
                    token_value_len++;
                    current_char = source[i];
                    if(current_char == 'r'){
                        printf("tu\n");
                        i++;
                        token_value_len++;
                        current_char = source[i];
                        if(current_char == 'i'){
                            printf("tu\n");
                            i++;
                            token_value_len++;
                            current_char = source[i];
                        }
                        if(current_char == 'n'){
                            printf("tu\n");
                            i++;
                            token_value_len++;
                            current_char = source[i];
                            if (current_char == 'g'){
                                token_storage_add(token_storage, TOK_KEYWORD, start_ptr, token_value_len);
                                i++;
                                current_state = STATE_START;
                            }
                        }
                    }
                }
                else{
                    return 1;
                }
                break;

            case STATE_EPILOG_2:
            // ?>..
                if (i + 1== source_len) {
                    return 0;
                }
                printf("i == %d, source_len == %d", i, source_len);
                return 2;

            case STATE_TERMINATE:
            return 0;
            break;
            case STATE_ERROR:
                if (DEBUG_LEXER) debug_print_state("STATE_ERROR", start_ptr, token_value_len);
                printf("%d\n", (int) *start_ptr);
                return 1;
            default:
                break;
        }
    }

    return 0;
}



// start state
state state_start(char c){
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
        i++;
        return STATE_START;
    } 
    else if(c == '$') {
        i++;
        return STATE_ID_START;
    }

    else if (is_digit(c)) {  
        i++;
        return STATE_LIT_NUM;
    }
    else if (is_alpha(c) || c == '_') {
        i++;
        return STATE_KEYWORD_MAIN;
    }
    else if (arr_contains_char(separators, c, separ_len)) {
        i++;
        return STATE_SEP;
    }
    else if (c == '/') {
        i++;
        return STATE_COMMENT_START;
    }
    else if (arr_contains_char(operators, c, oper_len)) {
        if (c == '=') {
            i++;
            token_value_len++;
            return STATE_EQUAL_1;
        }
        else if (c == '!') {
             i++;
            token_value_len++;
            return STATE_NOT_1;
        }
        else if (c == '<') {
            token_value_len++;
            i++;
            return STATE_SMALLER_EQUAL;     
        }
        else if (c == '>') {
            token_value_len++;
            i++;
            return STATE_GREATER_EQUAL;
        }
        return STATE_OP;
    }
    else if (c == '!') {
        i++;
        token_value_len++;
        return STATE_NOT_1;

    }
    else if (c == '"') {
        i++;
        token_value_len++;
        return STATE_LIT_STR;
    }
    else if (c == '?') {
        token_value_len++;
        i++;
        return STATE_EPILOG;
    }
    else if (c == '\0') {
        return STATE_TERMINATE;
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
    if (arr_contains_char(separators, c, separ_len) || arr_contains_char(operators, c, oper_len)) {
        return STATE_ID; 
    }
    return STATE_ERROR;
}

state state_keyword_main(char c) {
    if (is_alpha(c) || is_digit(c) || c == '_') {
        return STATE_KEYWORD_MAIN;
    }
    if (arr_contains_char(separators, c, separ_len) || arr_contains_char(operators, c, oper_len)) {
        return STATE_KEYWORD; 
    }
    return STATE_ERROR;
}


// get the next token from token_storage
token_t *get_token(token_storage_t *token_storage){
    if (token_index < token_storage->num_tokens) {
        if (DEBUG_PARSER) printf("%s\n", token_storage->tokens[token_index]->value);
        return token_storage->tokens[token_index++];

    }
    else {
        return NULL;
    }
}
token_t *get_token_keep(token_storage_t *token_storage){
    if (token_index < token_storage->num_tokens) {
        return token_storage->tokens[token_index];

    }
    else {
        return NULL;
    }
}
