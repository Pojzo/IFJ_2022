#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "utils.h"
#include "symtable.h"
//TODO errory su nekonzistentne, to este musime dohodnut ako ich dame 
/*
void id_node_init(id_node_t **node){
    node = NULL;
}*/

//inserting function
int insert_function_id(id_node_t* node, char* name, char *return_type, char **arguments ){
    //if inserting root
    if (node == NULL){
        node = malloc(sizeof(struct id_node));
        if(node == NULL){
            return 0;
        }
        node->name = name;
        node->left = NULL;
        node->right = NULL;
        node->return_type = return_type;
        node->arguments = arguments;
    }
    if(is_bigger(node->name,name)){
        insert_function_id(node->left, name, return_type, arguments);
    }
    else if(is_bigger(name, node->name)){
        insert_function_id(node->right, name, return_type, arguments);
    }
    else{
        //pokus o redefinicu funkcie - ERROR code 3
        return 1;
    }
    return 0;    
}


//inserting ID
//funckia sa bude volat iba v priradeni v parseri
int insert_id(id_node_t **node, char* name, char* datatype, char* scope){
    //if inserting root
    int error = 0;
   if (*(node) == NULL){
        *node = malloc(sizeof(struct id_node));
        if(*node == NULL){
            return 0;
        }
        (*node)->name = name;
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->datatype = datatype;
        (*node)->scope = scope;
        //print_tree(*node);
        //pozor pozor, psekulacia 
        return error;
    }
    //ked je vacsie idem do lava
    if(is_bigger((*node)->name, name)){
        insert_id(&(*node)->left, name, datatype, scope);
    }
    //ked je mensie idem do prava
    else if(is_bigger(name, (*node)->name)){
        insert_id(&(*node)->right, name, datatype, scope);
    }
    //ak sa rovnaju mena a aj scope
    else if(strcmp((*node)->scope,scope) == 0){
        //pokus o redefiniciu premennej v ramci scope s inym datovym typom
        if ((*node)->datatype != datatype){
            (*node)->datatype = datatype;
        }
    }
    //ak sa rovnaju mena id ale scope nie 
    else{
        printf("PREPADLI SME SEM BRASKO POMOC\n");
        insert_id(&(*node)->right, name, datatype, scope);
    }
    return error;    
}


//comparing strings alphabetically
bool is_bigger(char* a, char* b){
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0'){
        if(a[i] > b[i]){
            return true;
        }
        else if(a[i] < b[i]){
            return false;
        }
        i++;
    }
    if(a[i] == '\0'){
        return false;
    }
    else{
        return true;
    }
}


int check_if_declared(id_node_t* node, char* name, char* scope){
    if(node == NULL){
        return 0;
    }
    if(strcmp(node->name,name) == 0){
        if(strcmp(node->scope,scope) == 0){
            //ano brasko to id je deklarovane 
            return 1;
        }
        else{
            return check_if_declared(node->right,name,scope);
        }
    }
    else if(is_bigger(node->name,name)){
        return check_if_declared(node->left,name,scope);
    }
    else{
        return check_if_declared(node->right,name,scope);
    }
}

int check_if_function_declared(id_node_t* node, char* name, char **arguments){
    if(node == NULL){
        return 0;
    }
    if(strcmp(node->name,name) == 0){
        //ano brasko funckia je deklarovana
        return 1;
    }
    else if(is_bigger(node->name,name)){
        return check_if_function_declared(node->left,name,arguments);
    }
    else{
        return check_if_function_declared(node->right,name,arguments);
    }
}

//toto pojde do matky p boa (asi)
/*
bool is_equal(char* a, char* b){
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0'){
        if(a[i] != b[i]){
            return false;
        }
        i++;
    }
    if(a[i] == '\0' && b[i] == '\0'){
        return true;
    }
    else{
        return false;
    }
}
*/

void free_tree(id_node_t* node){
    if(node == NULL){
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

//function that prints all nodes of tree
void print_tree(id_node_t* node){
    if(node == NULL){
        return;
    }
    print_tree(node->left);
    printf("--------------------------------------\n");
    printf("NODE:\n"); 
    printf("name: %s\nscope: %s\ndatatype: %s\n",node->name, node->scope, node->datatype);
    print_tree(node->right);
}


char* get_data_type(token_t *token){
    if (token->token_type == TOK_LIT){
        if (strcmp(token->value, "NULL") == 0){
            return "VOID";
        }  
        else if(is_digit(token->value[0]) && arr_contains_char(token->value, '.', strlen(token->value))){
            return "FLOAT";
        }
        else if(is_digit(token->value[0])){
            return "INT";
            }
        else if(token->value[0] == '"'){
            return "STRING";
        }
    }
    return NULL;
}
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