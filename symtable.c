#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"


void id_node_init(id_node_t **node){
    *node = NULL;
}

//inserting function
void insert_function_id(id_node **node, char* name, data_type return_type, data_type **arguments ){
    //if inserting root
    if ((*node) == NULL){
        *node = malloc(sizeof(id_node_t));
        if((*node == NULL)){
            return;
        }
        (*node)->name = name;
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->return_type = return_type;
        (*node)->arguments = arguments;
    }
    if((*node)->name > name)

    
}

//create a function that will calculate ascii value of a string



/*
node_t *node_create(char *key, char *value) {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->left = NULL;
    node->right = NULL;
    node->key = key;
    node->value = value;
    return node;
}

void node_free(node_t *node) {
    free(node);
}

void node_print(node_t *node) {
    printf("Node with key %s has a value %s\nLeft child with key %s has value %s\nRight child with key %s has value %s\n\n", 
            node->key, node->value, node->left == NULL ? "NULL" : node->left->key, node->left == NULL ? "NULL" : node->left->value,
            node->right == NULL ? "NULL" : node->right->key, node->right == NULL ? "NULL" : node->right->value);
}

node_t *insert_item(node_t *head, char *key, char *value) {
    if (head == NULL) {
        return node_create(key, value);
    }
    if (strcmp(key, head->key) < 0) {
        head->left = insert_item(head->left, key, value);
    }

    if (strcmp(key, head->key) > 0) {
        head->right = insert_item(head->right, key, value);
    }

    return head;
}

void remove_item(node_t *head, const char *value) {
         
}

void search_item(node_t *head, const char *value) {

}











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

*/