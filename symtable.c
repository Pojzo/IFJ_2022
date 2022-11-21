#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

symtable_t *table_init() {
    symtable_t *table = malloc(sizeof(symtable_t));
    table->root = NULL;
    return table;
}

node_t *node_init() {
    node_t *node = malloc(sizeof(node_t));
    node->id = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}


void table_free(symtable_t *symtable) {
    free(symtable);
}

void node_free(node_t *node) {
    node_t *tmp = node;
    if (node->left != NULL) {
        node_free(node->left);
    }
    if (node->right != NULL) {
        node_free(node->right);
    }
    free(tmp);
}
