#ifndef SYMTABLE_H
#define SYMTABLE_H

// symbol table using a binary tree structure

typedef enum {INT, FLOAT, STRING, VOID} data_type;

//tree node 
typedef struct id_node {
    id_node *left;
    id_node *right;
    char *name;
    //id
    data_type datatype;
    char *scope;
    //function id
    data_type return_type;
    int num_arguments;
    data_type **arguments;
    
} id_node_t;


#endif
