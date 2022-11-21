#ifndef SYMTABLE_H
#define SYMTABLE_H

// symbol table using a binary tree structure

typedef struct node {
    char *id;
    struct node *left;
    struct node *right;
} node_t;

typedef struct Symtable {
    node_t *root;
} symtable_t;

node_t *node_init();
symtable_t *table_init();

void table_free();
void node_free();

#endif
