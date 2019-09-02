#include <stdio.h>
#include "linkedList.h"

Node *get_head(LinkedList *list) {
    return list->head;
}

Node *get_curr(LinkedList *list) {
    return list->current;
}

void remove_all_after_curr(LinkedList *list) {
    Node* temp = list->current->next;
    list->current->next = NULL;
    if(temp != NULL) {
        while (temp->next != NULL) {
            temp = temp->next;
            temp->prev->next=NULL;
            list->len--;
            free_node(temp->prev);
        }
        free_node(temp);
        list->len--;
    }

}


void add_linked_list(LinkedList *list, Command *c, LinkedListCells *old_values, LinkedListCells *new_values) {
    Node* newNode = alloc_node(c, old_values, new_values);
    if(list->current != NULL) {
        remove_all_after_curr(list);
        list->current->next = newNode;
        newNode->prev = list->current;
    }
    else{
        list->current = newNode;
        list->head = newNode;
    }
    list->len++;
}
Node *alloc_node(Command *c, LinkedListCells *old_values, LinkedListCells *new_values) {
    Node* newNode = (Node*) calloc(1, sizeof(Node));
    if(newNode==NULL){
        error("linkedList","alloc_node",1);
        exit(0);
    }
    newNode->c = c;
    newNode->old_values=old_values;
    newNode->new_values=new_values;
    return newNode;
}
LinkedList *alloc_linkedList() {
    LinkedList* newList = (LinkedList*) calloc(1, sizeof(LinkedList));
    if(newList == NULL){
        error("linkedList","alloc_linkedList",1);
        exit(0);
    }
    return newList;
}
void advance_curr(LinkedList *list) {
    if(list->current == NULL){
        error("linkedList","advance_curr",2);
        exit(0);
    }
    if(list->current->next==NULL){
        error("linkedList","advance_curr",3);
        exit(0);
    }
    list->current = list->current->next;
}
void backward_curr(LinkedList *list) {
    if(list->current == NULL){
        error("linkedList","backward_curr",2);
        exit(0);
    }
    if(list->current->prev==NULL){
        error("linkedList","backward_curr",4);
        exit(0);
    }
    list->current=list->current->prev;
}


void free_node(Node *node) {
    if(node!= NULL) {
        free_linked_list_cells(node->old_values);
        free_linked_list_cells(node->new_values);
        free_command(node->c);
        free(node);
    }
}

void free_linked_list(LinkedList *list) {
    Node* temp;
    if(list != NULL) {
        temp=list->head;
        while(temp->next != NULL){
            temp = temp->next;
        }
        while(temp != list->head){
            temp=temp->prev;
            free_node(temp->next);
        }
        free_node(temp);
        free(list);
    }
}

Command *get_command(Node *node) {
    return node->c;
}

int is_curr_last(LinkedList *list) {
    if (list->current->next==NULL)
        return 1;
    return 0;
}

LinkedListCells * get_old_values_cells_list(Node *node) { return node->old_values; }
LinkedListCells * get_new_values_cells_list(Node *node) { return node->new_values; }

void clear_linked_list(LinkedList *list) {
    while (get_curr(list) != get_head(list))
        backward_curr(list);
    remove_all_after_curr(list);
}


