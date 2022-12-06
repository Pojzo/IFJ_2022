/*

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    list_t *list = list_init();
    print_list(list);
    list_insert_after_nonterm(&list);
    print_list(list);
    list_insert_first(&list, INT);
    print_list(list);
    int num = symbols_till_stop(list);
    printf("Tolkoto je tam symbolov %d\n", num);
    list_pop_first(&list);
    list_pop_first(&list);
    print_list(list);
    list_insert_first(&list, NONTERM);
    print_list(list);
    list_insert_after_nonterm(&list);
    list_insert_first(&list, ADD);
    print_list(list);
    list_insert_after_nonterm(&list);
    list_insert_first(&list, INT);
    print_list(list);
    return 0;
}
*/
