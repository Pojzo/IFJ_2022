#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

#define assert_not_null(_list) assert(_list != NULL)

const char *symbol_to_string[] = {
    "INT",
    "FLOAT",
    "STRING",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "CONC",
    "NEQ",
    "EQ",
    "GT",
    "LT",
    "GTE",
    "LTE",
    "DOLLAR",
    "OPENBR",
    "CLOSEDBR",
    "STOP",
    "NONTERM",
};

list_t* list_init() {
    list_t *list = malloc(sizeof(list_t));
    //list->element = malloc(sizeof(element_t));
    //list->element->token = NULL;
    //list->element->symbol = DOLLAR;
    list->symbol = DOLLAR;
    list->next = NULL;
    return list;
}

void list_free(list_t *list) {
    assert_not_null(list);
    list_t *cur = list;
    while(cur != NULL) {
        list_t *next = cur->next;
     //   free(cur->element);
        free(cur);
        cur = next;
    }
}

// insert element to first position of the list and change its head
void list_insert_first(list_t **list, symbol_enum symbol) {
    list_t *new = (list_t *) malloc(sizeof(list_t));
    new->symbol = symbol;

    if (*list == NULL) {
        printf("tu som sa ani nemal dostat more\n");
        *list = new;
        return;
    }

    new->next = *list;
    *list = new;
}


void list_insert_after_nonterm(list_t **list) {
    assert_not_null(list);
   	list_t* prev = NULL;
    list_t *cur = *list;
    while (cur != NULL) {
        if (cur->symbol < STOP) {
            if (prev == NULL) {
                list_insert_first(list, STOP);
                return;
            }
            else {
                list_t *new_node = malloc(sizeof(list_t));
                new_node->symbol = STOP;
                prev->next = new_node;
                new_node->next = cur;
                return;
            }
        }
        else {
            prev = cur;
            cur = cur->next;
        }
    }
}

symbol_enum list_get_first_term(list_t *list) {
    assert_not_null(list);
    list_t *cur = list;
    while (cur != NULL) {
        if (cur->symbol < STOP) {
            return cur->symbol;
        }
        else {
            cur = cur->next;
        }
    }
    return DOLLAR;
}

symbol_enum list_pop_first(list_t **list) {
    assert_not_null(list);
    symbol_enum to_return = (*list)->symbol;
    list_t* todelete = *list;
    *list = todelete->next;
    free(todelete);
    todelete = NULL;
    return to_return;
}


// checks whether the list only contains $E
bool final_condition(list_t *list) {
    assert_not_null(list);
    //printf("final_condition: %s, %s, %s\n", symbol_to_string[list->symbol], symbol_to_string[list->next->symbol], symbol_to_string[list->next->next->symbol]);
    return (list->symbol == NONTERM && list->next->symbol == DOLLAR);
}
// print list
void print_list(list_t *list) {
    assert_not_null(list);
    list_t *cur = list;
    while (cur != NULL) {
        printf("%s ", symbol_to_string[cur->symbol]);
        cur = cur->next;
    }
    printf("\n");
}

int symbols_till_stop(list_t* list) {
    int num = 0;
    while(list->symbol != STOP) {
        num++;
        list = list->next;
        if (list == NULL) {
            // nieco sa vazne pokazilo
            return -1;
        }
    }
    return num;
}

int return_before_stop(list_t** list, symbol_enum* symbol1, symbol_enum* symbol2, symbol_enum* symbol3, int* num) {
    list_t* curr = *list;
    *num = 0;
    while(curr->symbol != STOP)
    {
        if(*num == 0) {
            *symbol1 = curr->symbol;
            list_pop_first(list);
        }
        else if(*num == 1) {
            *symbol2 = curr->symbol;
            list_pop_first(list);
        }
        else if(*num == 2) {
            *symbol3 = curr->symbol;
            list_pop_first(list);
        }
        else {
            return 0;
        }
        
        (*num)++;
        if (curr == NULL) {
            return 0;
        }
        if (curr->next == NULL) {
            return 0;
        }
        curr = curr->next;
        
    }
    list_pop_first(list);
    if (*num != 1 && *num != 3)
    {
        return 0; 
    }
    return 1;
}
