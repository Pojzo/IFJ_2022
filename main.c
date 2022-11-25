#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "parser.h"
#include "tokenizer.h"

#define MAX_BUFFER_LEN 1000

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


int main() {
    char *buffer = get_buffer();
    if (buffer == NULL) {
        return 1;
    }
    token_storage_t *token_storage = token_storage_create();
    int error = dka(buffer, strlen(buffer), token_storage);
    for(int i = 0; i < token_storage->num_tokens; i++) {
        token_print(token_storage->tokens[i]);
    }
    printf("Length of token storage -> %d\n", token_storage->num_tokens);

    token_storage_free(token_storage);
    if (error) {
        printf("[ERROR] An error has occured in lexical analysis %s\n", "\U0001F913");
    }
    free(buffer);
    return error;
}
