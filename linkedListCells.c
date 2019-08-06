
#include "linkedListCells.h"
#include <stdio.h>

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
        printf("free_node_cell: ");
        free(node->cell);
        printf("1 ; ");
        free(node->prev);
        printf("2 ; ");
        free(node->next);
        printf("3 ; ");
    }
    printf("4\n");
    free(node);
}

void free_linked_list_cells(LinkedListCells *list) {
    NodeCell* temp;
    int i;
    if(list!=NULL) {
        temp = list->head;
        if (temp != NULL) {
            for (i = 0; i < list->len - 1; i++) {
                temp = temp->next;
                printf("free_linked_list_cells: before free_node_cell 1 \n");
                free_node_cell(temp->prev);
                printf("free_linked_list_cells: after free_node_cell 1 \n");
            }
            printf("free_linked_list_cells: before free_node_cell 2 \n");
            free_node_cell(list->head);
            printf("free_linked_list_cells: after free_node_cell 2 \n");
        }
        free(list);
    }
}

Cell *get_head_cell(LinkedListCells *list) {
    return list->head->cell;
}

int next_cell(LinkedListCells *list) {
    if (list->current->next == NULL) {
        error("linkedListCells","next_cell",3);
        return 0;
    }
    list->current = list->current->next;
    return 1;
}

int prev_cell(LinkedListCells *list) {
    if(list->head = list->current) {
        error("linkedListCells","prev_cell",4);
        return 0;
    }
    list->current = list->current->prev;
    return 1;
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

void print_linked_list_cells(LinkedListCells *list) {
    NodeCell* temp = list->head;
    Cell* temp_cell;
    while(temp->next!=NULL){
        temp_cell= temp->cell;
        printf("[%d][%d]: %d ,",get_cell_row(temp_cell),get_cell_col(temp_cell),get_cell_value(temp_cell));
        temp=temp->next;
    }
    temp_cell= temp->cell;
    printf("[%d][%d]: %d\n",get_cell_row(temp_cell),get_cell_col(temp_cell),get_cell_value(temp_cell));
}
