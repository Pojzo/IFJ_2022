#include <stdio.h>
#include <string.h>
#include "tokenizer.h"
#include <assert.h> 

#define MAX_BUFFER_LEN 1000

#define DEBUG

int main() {
    FILE *fp;
    fp = fopen("input", "r");
    if (ferror(fp)) {
        printf("Couldn't open input file\n");
        fclose(fp);
        return 1;
    }
    char buffer[MAX_BUFFER_LEN];
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

    return error;
}
