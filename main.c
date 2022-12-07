/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "tokenizer.h"

const int MAX_BUFFER_LEN = 10000;

const int DEBUG_PARSER = 1;
const int DEBUG_LEXER = 0;


char *get_buffer();
int run(char *buffer);
void test_prolog();

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

    char *source = get_stdin();
    printf("%s", source);
    int error = run(source);
    free(source);
    printf("Returnujem tento error %d\n", error);
    exit(error);
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
