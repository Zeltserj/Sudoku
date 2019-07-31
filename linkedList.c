#include <stdio.h>
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
        list->current->next = NULL;
        free_node(next);
        list->len--;
        return 1;
    }
    list->current->next = list->current->next->next;
    list->current->next->next->prev = list->current;
    free_node(next);
    list->len--;
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
    free_node(node);
    list->len--;

}

void remove_all_after_curr(LinkedList *list) {
    Node* temp = list->current->next;
    list->current->next = NULL;
    if(temp != NULL) {
        while (temp->next != NULL) {
            temp = temp->next;
            free_node(temp->prev);
            list->len--;
        }
        free_node(temp);
        list->len--;
    }

}


void add_linked_list(LinkedList *list, Command *c, LinkedListCells *changed, int prevmode) {
    Node* newNode = alloc_node(c, changed, prevmode);
    if(list->current != NULL) {
        remove_all_after_curr(list);
        list->current->next = newNode;
        newNode->prev = list->current;
        list->len++;
    }
    else{
        list->len++;
        list->current = newNode;
        list->head = newNode;
    }

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
LinkedList *alloc_linkedList() {
    LinkedList* newList = calloc(3, sizeof(int*));
    if(newList == NULL){
        error("linkedList","alloc_linkedList",1);
        exit(0);
    }
}

int get_prev_mode(Node *node) {
    return node->prevmode;
}

void free_node(Node *node) {
    if(node!= NULL) {
        free_linked_list_cells(node->changed);
        free_command(node->c);
        free(node);
    }
}

void free_linked_list(LinkedList *list) {
    Node* temp;
    int i;
    if(list != NULL) {
        temp = list->head;
        if(temp!=NULL) {
            for (i = 0; i < list->len - 1; i++) {
                temp = temp->next;
                free(temp->prev);
            }
            free_node(list->head);
        }
        free(list);
    }
}

void print_linked_list_prevmode(LinkedList* list){
    Node* temp=list->head;
    printf("Linked list:\n");
    while(temp!=NULL){
        printf("%d, ",temp->prevmode);
        temp=temp->next;
    }
    printf("\n");
}


