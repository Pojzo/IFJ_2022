#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "utils.h"
#include "symtable.h"
//TODO errory su nekonzistentne, to este musime dohodnut ako ich dame 


//inserting function
int insert_function_id(id_node_t** node, char* name){
    //if inserting root
    if ((*node) == NULL){
        (*node) = malloc(sizeof(struct id_node));
        if((*node) == NULL){
            return 0;
        }
        (*node)->name = name;
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->return_type = TYPE_VOID;
        (*node)->arguments = NULL;
        (*node)->num_arguments = 0;
        return 1;
    }
    if(is_bigger((*node)->name,name)){
        insert_function_id(&(*node)->left, name);
    }
    else if(is_bigger(name, (*node)->name)){
        insert_function_id(&(*node)->right, name);
    }
    return 3;   
}


//inserting ID
//funckia sa bude volat iba v priradeni v parseri
int insert_id(id_node_t **node, char* name, datatype_t datatype , char* scope){
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

id_node_t* search(id_node_t* node, char* name){
    if(node == NULL){
        return 0;
    }
    if(strcmp(node->name, name) == 0){
        return node;
    }
    else if(is_bigger(node->name, name)){
        return search(node->left, name);
    }
    else{
        return search(node->right, name);
    }
}



int fun_add_arg(id_node_t* node, char* scope, datatype_t datatype) {
    // print the datatype as string
    id_node_t* current = search(node, scope);
    current->num_arguments++;
    current->arguments = realloc(current->arguments, current->num_arguments * sizeof(enum datatype));
    current->arguments[current->num_arguments - 1] = datatype;
    return 0;
}

int fun_add_return_type(id_node_t* node, char* scope, datatype_t datatype) {
    // print the datatype as string
    id_node_t* current = search(node, scope);
    current->return_type = datatype;
    return 0;
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
    if(node->name[0] == '$'){
        printf("name: %s\nscope: %s\ndatatype: %s\n",node->name, node->scope, convert_back(node->datatype));
    }
    else{
        printf("name: %s\nreturntype: %s\n",node->name, convert_back(node->return_type));
        // print all arguments of function along with the function's name
        for (int i = 0; i < node->num_arguments; i++) {
            printf("argument %d: %s\n", i, convert_back(node->arguments[i]));
        }
        
    }
    print_tree(node->right);
}


datatype_t convert_char_to_datatype(char *type) {
    if (strcmp(type, "int") == 0) {
        return TYPE_INT;
    }
    if (strcmp(type, "float") == 0) {
        return TYPE_FLOAT;
    }
    if (strcmp(type, "string") == 0) {
        return TYPE_STRING;
    }
    return TYPE_VOID;
}

char *convert_back(datatype_t type) {
    if (type == TYPE_INT) {
        return "int";
    }
    if (type == TYPE_FLOAT) {
        return "float";
    }
    if (type == TYPE_STRING) {
        return "string";
    }
    return "void";
}