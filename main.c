#include <stdio.h>
#include "board.h"
#include "game.h"
#include "linkedList.h"

int main() {

    LinkedList *list = alloc_linkedList();
    add_linked_list(list, NULL, NULL, 4);
    add_linked_list(list, NULL, NULL, 5);
    print_linked_list_prevmode(list);
    printf("curr: %d\n", list->current->prevmode);

    advance_curr(list);
    printf("curr: %d\n", list->current->prevmode);

    add_linked_list(list, NULL, NULL, 7);
    advance_curr(list);
    add_linked_list(list, NULL, NULL, 6);
    print_linked_list_prevmode(list);
    printf("curr: %d\n", list->current->prevmode);
    remove_next_after_curr(list);
    printf("curr: %d\n", list->current->prevmode);

    print_linked_list_prevmode(list);


}
