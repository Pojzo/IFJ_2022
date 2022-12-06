#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "tokenizer.h"

const int MAX_BUFFER_LEN = 1000;

const int DEBUG_PARSER = 0;
const int DEBUG_LEXER = 0;


char *get_buffer();
int run(char *buffer);
void test_prolog();

// comment the code below
char *get_buffer() {
    FILE *fp;
    fp = fopen("input.txt", "r");
    if (ferror(fp)) {
        printf("Couldn't open input file\n");
        fclose(fp);
        return NULL;
    }
    char *buffer = malloc(MAX_BUFFER_LEN);
    char c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF) {
        buffer[i++] = c;
    }
    buffer[i] = ' ';
    buffer[i+1] = '\0';
    // printf("%s\n", buffer);
    // printf("%d\n", (int)strlen(buffer));
    fclose(fp);

    return buffer;
}

// write a function to load standard input into a buffer and return it 
// (you can use malloc to allocate memory for the buffer)
// comment the code below

// using the function fgets, read the standard input into a buffer and return it
// account for new lines, get the whole input

char *get_stdin() {
    char *buffer = malloc(MAX_BUFFER_LEN);
    int c;
    size_t s = 0;
    while ((c = getchar()) != EOF) {
        buffer[s] = c;
        s++;
    }
    buffer[s] = '\n';
    printf("Na konci %s to newline\n", buffer[s] == '\n' ? "je" : "nie je ");
    return buffer;
}


const char *prolog = "<?php";
const char *epilog = "?>";

int main() {

    // char *source = get_buffer();
    char *source = get_stdin();
    printf("%s", source);
    int error = run(source);
    free(source);
    /*
       printf("Running prolog tests\n-------------------\n");
       test_prolog();
       */
    printf("Returnujem tento error %d\n", error);
    return error;
}

int run(char *source) {
    // input source is too short
    if (strlen(source) < strlen(prolog)) {
        return 1;
    }

    int error = parser_start(source);

    // printf("error value: %d\n", error);

    return error;
}

const char *test_programs[] = {
    "<?php\ndeclare(strict_types=1);",
    "<?php\ndeclare(strict_types=1);",
    "<?php declare(strict_types=1);"
        "<?php\n\ndeclare(strict_types=1);",
    "<?php//komentar\ndeclare(strict_types=1);",
    "<?php/*komentar*/declare(strict_types=1);"};

const int num_test_programs = 5;
