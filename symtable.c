
/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "utils.h"
#include "symtable.h"

extern int error;
// inserting function
int insert_function_id(id_node_t **node, char *name)
{
    // if inserting root
    if ((*node) == NULL)
    {
        (*node) = malloc(sizeof(struct id_node));
        if ((*node) == NULL)
        {
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
    if (is_bigger((*node)->name, name))
    {
        return insert_function_id(&(*node)->left, name);
    }
    else if (is_bigger(name, (*node)->name))
    {
        return insert_function_id(&(*node)->right, name);
    }
    return 3;
}

// inserting ID
// function will be called only in parser
int insert_id(id_node_t **node, char *name, datatype_t datatype, char *scope) {
    // if inserting root
    int error = 0;
    if (*(node) == NULL){
        *node = malloc(sizeof(struct id_node));
        if (*node == NULL){
            return 0;
        }
        (*node)->name = name;
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->datatype = datatype;
        (*node)->scope = scope;
        (*node)->arguments = NULL;
        (*node)->num_arguments = 0;
        return error;
    }
    // if bigger, go left
    if (is_bigger((*node)->name, name)){
        insert_id(&(*node)->left, name, datatype, scope);
    }
    // if smaller, go right
    else if (is_bigger(name, (*node)->name)){
        insert_id(&(*node)->right, name, datatype, scope);
    }
    //
    else if (strcmp((*node)->scope, scope) == 0){
        if ((*node)->datatype != datatype){
            (*node)->datatype = datatype;
        }
    }
    else
    {
        insert_id(&(*node)->right, name, datatype, scope);
    }
    return error;
}


//insert function parameters
int insert_param_id(id_node_t **node, char *name, datatype_t datatype, char *scope)
{

    if (*(node) == NULL)
    {
        *node = malloc(sizeof(struct id_node));
        if (*node == NULL)
        {
            return 0;
        }
        (*node)->name = name;
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->datatype = datatype;
        (*node)->scope = scope;
        (*node)->arguments = NULL;
        (*node)->num_arguments = 0;
        return error;
    }
    // if bigger, go left
    if (is_bigger((*node)->name, name))
    {
        insert_param_id(&(*node)->left, name, datatype, scope);
    }
        // if smaller, go right
    else if (is_bigger(name, (*node)->name))
    {
        insert_param_id(&(*node)->right, name, datatype, scope);
    }
    else if (strcmp((*node)->scope, scope) == 0)
    {
        error = 4;
        return error;
    }
    else
    {
        insert_id(&(*node)->right, name, datatype, scope);
    }
    return error;
}
// comparing strings alphabetically
bool is_bigger(char *a, char *b)
{

    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] > b[i])
        {
            return true;
        }
        else if (a[i] < b[i])
        {
            return false;
        }
        i++;
    }
    if (a[i] == '\0')
    {
        return false;
    }
    else
    {
        return true;
    }
}

int check_if_declared(id_node_t *node, char *name, char *scope) {
    if (node == NULL) {
        return 0;
    }
    if (strcmp(node->name, name) == 0) {
        if (strcmp(node->scope, scope) == 0) {
            return 1;
        }
        else {
            return check_if_declared(node->right, name, scope);
        }
    }
    else if (is_bigger(node->name, name)) {
        return check_if_declared(node->left, name, scope);
    }
    return check_if_declared(node->right, name, scope);
}

// explicitne pre funkcie funguje
datatype_t search_return_type(id_node_t *node, char *name){
    if (node == NULL) {
        return -1;
    }
    if (is_bigger(node->name, name)) {
        return search_return_type(node->left, name);
    }
    else if (is_bigger(name, node->name)){
        return search_return_type(node->right, name);
    }
    else {
        return node->return_type;
    }
}

datatype_t search_datatype(id_node_t *node, char *name, char*scope){
    if (node == NULL) {
        return -1;
    }
    if (is_bigger(node->name, name)) {
        return search_datatype(node->left, name, scope);
    }
    else if (is_bigger(name, node->name)) {
        return search_datatype(node->right, name, scope);
    }
    else {
        if(strcmp(node->scope, scope) == 0){
            return node->datatype;
        }
        else{
            return search_datatype(node->right, name, scope);
        }
    }
}

id_node_t *search(id_node_t *node, char *name){
    if (node == NULL) {
        return NULL;
    }
    if (strcmp(node->name, name) == 0) {
        return node;
    }
    else if (is_bigger(node->name, name)) {
        return search(node->left, name);
    }
    else {
        return search(node->right, name);
    }
}


int fun_add_arg(id_node_t *node, char *scope, datatype_t datatype){
    if (node == NULL){
        return 0;
    }
    id_node_t *current = search(node, scope);
    current->num_arguments++;
    current->arguments = realloc(current->arguments, current->num_arguments * sizeof(enum datatype));
    current->arguments[current->num_arguments - 1] = datatype;
    return 0;
}

int fun_add_return_type(id_node_t *node, char *scope, datatype_t datatype){
    id_node_t *current = search(node, scope);
    current->return_type = datatype;
    return 0;
}

// write a function to recursively free the tree
void free_tree(id_node_t *node){
    if (node == NULL){
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    free(node->arguments);
    free(node);
}

datatype_t convert_char_to_datatype(char *type){
    if (strcmp(type, "int") == 0){
        return TYPE_INT;
    }
    if (strcmp(type, "float") == 0){
        return TYPE_FLOAT;
    }
    if (strcmp(type, "string") == 0){
        return TYPE_STRING;
    }
    if (strcmp(type, "?int") == 0){
        return TYPE_OPT_INT;
    }
    if (strcmp(type, "?float") == 0){
        return TYPE_OPT_FLOAT;
    }
    if (strcmp(type, "?string") == 0){
        return TYPE_OPT_STRING;
    }
    return TYPE_VOID;
}

char *convert_back(datatype_t type)
{
    if (type == TYPE_INT)
    {
        return "int";
    }
    if (type == TYPE_FLOAT)
    {
        return "float";
    }
    if (type == TYPE_STRING)
    {
        return "string";
    }
    if (type == TYPE_OPT_INT)
    {
        return "?int";
    }
    if (type == TYPE_OPT_FLOAT)
    {
        return "?float";
    }
    if (type == TYPE_OPT_STRING)
    {
        return "?string";
    }
    return "void";
}
