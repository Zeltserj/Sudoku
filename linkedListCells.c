
#include "linkedListCells.h"

void add_cell_after_curr(LinkedListCells *list, Cell *c) {
    NodeCell* newNode;
    NodeCell* temp;
    newNode = alloc_node_cell(c);
    if(list->current!= NULL) {
        temp = list->current->next;
        if (temp != NULL) {
            list->current->next->prev = newNode;
        }
        list->current->next = newNode;
        newNode->prev = list->current;
        newNode->next = temp;

    }
    else{
        list->head = newNode;
        list->current = list->head;
    }
    list->len++;
}

Cell *get_curr_cell(LinkedListCells *list) {
    return list->current->cell;
}

NodeCell * alloc_node_cell(Cell* c) {
    NodeCell* nodeC = (NodeCell*) calloc(1, sizeof(NodeCell));
    if(nodeC == NULL){
        error("linkedListCells","alloc_node_cell",1);
        exit(0);
    }
    nodeC->cell = c;
    return nodeC;
}

LinkedListCells *alloc_linked_list_cells() {
    LinkedListCells* new_list = (LinkedListCells*) calloc(1, sizeof(LinkedListCells));
    if(new_list == NULL){
        error("linkedListCells","alloc_linked_list_cells",1);
        exit(0);
    }
    return new_list;
}

void free_node_cell(NodeCell *node) {
    if(node != NULL){
        free(node->cell);
    }
    free(node);
}

void free_linked_list_cells(LinkedListCells *list) {
    NodeCell* temp;
    if(list!= NULL){
        if(get_len_linked_list_cells(list) > 0) {
            temp = list->head;
            while (temp->next != NULL)
                temp = temp->next;
            while (temp != list->head) {
                temp = temp->prev;
                free_node_cell(temp->next);
            }
            free_node_cell(temp);
        }
        free(list);
    }
}


void move_curr_to_head(LinkedListCells *list) {
    list->current=list->head;
}

int advance_curr_cell(LinkedListCells *list) {
    if(list->current->next == NULL){
        return  0;
    }
    list->current = list->current->next;
    return 1;
}

int get_len_linked_list_cells(LinkedListCells *list) {
    return list->len;
}

