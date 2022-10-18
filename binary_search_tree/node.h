#ifndef NODE_H
#define NODE_H

typedef struct Node {
    char *key;
    char *value;
    struct Node *left;
    struct Node *right;
} node_t;

node_t *node_create();
void node_free();
void node_print(node_t *node);
node_t *insert_item(node_t *head, char *item, char *value);
void remove_item(node_t *head, const char *item);
const char *search(const node_t *head, const char *item);

#endif
