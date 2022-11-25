#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

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


int main() {
    char *buffer = get_buffer();
    if (buffer == NULL) {
        return 1;
    }
    int error = parser_start(buffer);

    free(buffer);
    return error;
}
