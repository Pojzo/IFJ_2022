#include <stdio.h>
#include <stdlib.h>

#include "node.h"

int main() {
    node_t *head = node_create("Peter", "20");
    insert_item(head, "Veronika", "15");
    insert_item(head, "Eva", "48");
    insert_item(head, "Davido", "20");
    insert_item(head, "Marek", "20");
    node_print(head);
    printf("\n");
    node_print(head->left);
    node_print(head->right);
    return 0;
}
