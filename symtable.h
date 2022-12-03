#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdbool.h>
// symbol table using a binary tree structure


//tree node 
typedef struct id_node {
    struct id_node *left;
    struct id_node *right;
    char *name;
    //id
    char* datatype;
    char* scope;
    //function id
    char* return_type;
    int num_arguments;
    char **arguments;
    
} id_node_t;

int insert_function_id(id_node_t* node, char* name, char* return_type, char** arguments );
int insert_id(id_node_t** node, char* name, char* datatype, char* scope);
bool is_bigger(char* a, char* b);
int check_if_declared(id_node_t* node, char* name, char* scope);
int check_if_function_declared(id_node_t* node, char* name, char **arguments);
void free_tree(id_node_t* node);
void print_tree(id_node_t* node);
#endif
