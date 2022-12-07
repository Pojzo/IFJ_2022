/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdbool.h>
// symbol table using a binary tree structure

typedef enum datatype {
    TYPE_INT,
    TYPE_OPT_INT,
    TYPE_FLOAT,
    TYPE_OPT_FLOAT,
    TYPE_STRING,
    TYPE_OPT_STRING,
    TYPE_VOID
} datatype_t;

//tree node 
typedef struct id_node {
    struct id_node *left;
    struct id_node *right;
    char *name;
    //id
    datatype_t datatype;
    char* scope;
    //function id
    datatype_t return_type;
    int num_arguments;
    datatype_t *arguments;
    
} id_node_t;



int insert_function_id(id_node_t** node, char* name);
int insert_id(id_node_t** node, char* name, datatype_t datatype, char* scope);
bool is_bigger(char* a, char* b);
int check_if_declared(id_node_t* node, char* name, char* scope);
id_node_t* search(id_node_t* node, char* name);
void free_tree(id_node_t* node);
void print_tree(id_node_t* node);
int fun_add_arg(id_node_t* node, char* scope, datatype_t datatype);
int fun_add_return_type(id_node_t* node, char* scope, datatype_t datatype);
datatype_t convert_char_to_datatype(char *type);
char *convert_back(datatype_t type);

#endif
