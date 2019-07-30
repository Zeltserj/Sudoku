#include <stdlib.h>
#include "linkedList.h"

Node *get_head(LinkedList *list) {
    return list->head;
}

Node *get_curr(LinkedList *list) {
    return list->current;
}

int remove_next_after_curr(LinkedList *list) {
    Node *next = list->current->next;
    if (list->current == NULL) {
        error("linkedList","remove_next_after_curr",2);
        exit(0);
    }
    if (list->current->next == NULL) {
        return 1;
    }
    if (list->current->next->next == NULL) {
        free_node(next);
        free(list->current->next);
        return 1;
    }
    list->current->next = list->current->next->next;
    list->current->next->next->prev = list->current;
    free_node(next);
    free(next);
    return 1;
}

void remove_node(LinkedList *list, Node *node) {
    if (list->head == node) {
        list->head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if(node->next != NULL){
        node->next->prev = node->prev;
    }
    if(list->current == node){
        if(list->current == list->head){
            list->current = node->next;
        }
        else{
            list->current = node->prev;
        }
    }
    free(node);

}

void remove_all_after_curr(LinkedList *list) {
    Node* temp = list->current->next;
    if(temp != NULL) {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        while (temp != list->current) {
            temp = temp->prev;
            free_node(temp->next);
            list->len--;
        }
    }
}
/*TODO: Or: write after linkedListCells */
void free_linked_list_cell(LinkedListCells *changed) {

}
void free_node(Node *node) {
    free(node->next);
    free(node->prev);
    free_linked_list_cell(node->changed);
    free(node->changed);
    free_command(node->c);
    free(node->c);
}


void add(LinkedList *list, Command *c, LinkedListCells *changed, int prevmode) {
    Node* newNode = alloc_node(c, changed, prevmode);
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
Node * alloc_node(Command *c, LinkedListCells *changed, int prevmode){
    Node* newNode = calloc(5, sizeof(int*));
    if(newNode==NULL){
        error("linkedList","alloc_node",1);
        exit(0);
    }
    newNode->c = c;
    newNode->changed=changed;
    newNode->prevmode = prevmode;

}
int advance_curr(LinkedList *list) {
    if(list->current == NULL){
        error("linkedList","advance_curr",3);
        exit(0);
    }
    list->current = list->current->next;
    return 1;
}

LinkedList *alloc_linkedList() {
    LinkedList* newList = calloc(3, sizeof(int*));
    if(newList == NULL){
        error("linkedList","alloc_linkedList",1);
        exit(0);
    }
}
