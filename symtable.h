#ifndef SYMTABLE_H
#define SYMTABLE_H

// symbol table using a binary tree structure

typedef enum {INT, FLOAT, STRING, VOID} data_type;

//tree node 
typedef struct id_node {
    struct id_node *left;
    struct id_node *right;
    char *name;
    //id
    data_type datatype;
    char *scope;
    //function id
    data_type return_type;
    int num_arguments;
    data_type **arguments;
    
} id_node_t;

int insert_function_id(id_node_t** node, char* name, data_type return_type, data_type **arguments );
int insert_id(id_node_t** node, char* name, data_type datatype, char* scope);
bool is_bigger(char* a, char* b);
int check_if_declared(id_node_t* node, char* name, char* scope);
int check_if_function_declared(id_node_t* node, char* name, data_type **arguments);
void free_tree(id_node_t* node);
#endif
