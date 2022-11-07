#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef struct node {
    char *id;
    struct node *left;
    struct node *right;
} node_t;

typedef struct Symtable {
    node_t *root;
} symtable_t;

symtable_t *table_init();

void table_free();

#endif
