#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../tokenizer.h"
#include "tests.h"

extern const char* keywords[];
extern const int keywords_len;
extern bool is_keyword(char *, int);

// store tests in an array
const int NUM_TESTS = 1;
int (*tests[1])() = {&TEST_KEYWORDS};

int main() {
    int correct_tests = 0;
    for (int i = 0; i < NUM_TESTS; i++) {
        correct_tests += tests[i]();
    }
    printf("%d/%d tests were successful\n", correct_tests, NUM_TESTS);
}

// return 1 if all tests were successful
int TEST_KEYWORDS() {
    int correct_keywords = 0;
    for (int i = 0; i < keywords_len; i++) {
        correct_keywords += is_keyword((char *) keywords[i], strlen(keywords[i]));
    }
    return correct_keywords == keywords_len;
}