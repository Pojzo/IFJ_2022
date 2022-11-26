#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "tokenizer.h"

const int MAX_BUFFER_LEN = 1000;

#define DEBUG

// comment the code below
char *get_buffer() {
    FILE *fp;
    fp = fopen("input", "r");
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
    printf("%s\n", buffer);
    printf("%d\n", (int)strlen(buffer));
    fclose(fp);

    return buffer;
}

const char *prolog = "<?php";


// check if buffer starts with prolog
bool check_prolog(char *buffer) {
    for (int i = 0; i < strlen(prolog); i++) {
        if (buffer[i] != prolog[i]) {
            return false;
        }
    }
    return true;
}


int main() {
    char *source = get_buffer();
    if (source == NULL) {
        return 1;
    }
    // input source is too short
    if (strlen(source) < strlen(prolog)) {
        return 1;
    }

    if (!(check_prolog(source))) {
        printf("[ERROR] An error has occured in syntax analysis %s\n", "\U0001F913\n");
        return 2;
    }

    //moving prolog out of source
    char *buffer = source + strlen(prolog);
    
    int error = parser_start(buffer);

    printf("error value: %d\n", error);

    free(buffer);
    return error;
}
