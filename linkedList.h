
#ifndef SP_LINKEDLIST_H
#define SP_LINKEDLIST_H

#include <stdlib.h>
#include "linkedListCells.h"
#include "command.h"

typedef struct NodeLinkedList{
    Command* c;
    LinkedListCells* changed;
    int prevmode; /*TODO: Or: unnecessary?*/
    struct NodeLinkedList* next;
    struct NodeLinkedList* prev;
}Node;

typedef struct LinkedList{
    Node* head;
    Node* current;
    int len;
}LinkedList;

/**
* @param list - with pointers to its head and to a current node
* @return the first node of the list
*/
Node *get_head(LinkedList *list);

/**
* @param list - with pointers to its head and to a current node
* @return the curr node of the list
*/
Node* get_curr(LinkedList *list);

int is_curr_last(LinkedList* list);
Command* get_command(Node* node);
/**
* @param node != NULL
* @return the game mode before node's command was executed
*/
int get_prev_mode(Node* node);
/**
* adds after current node a new node with input arguments as properties.
* if current == NULL (the list is empty) than node will become current and head.
* @param list of nodes from Node type
* @param c - the executed command
* @param changed - a linked list of the cells which had been changed due to the command execution
* @param prevmode - the game mode before the command execution
*/
void add_linked_list(LinkedList *list, Command *c, LinkedListCells *changed, int prevmode);

/**
* @param c - the executed command
* @param changed - a linked list of the cells which had been changed due to the command execution
* @param prevmode - the game mode before the command execution
* @return pointer to a new node with input arguments as properties. next and prev are both NULL at that point
*/
Node * alloc_node(Command *c, LinkedListCells *changed, int prevmode);
/**
* allocates memory for linked list type.
* @return a pointer to the empty linked list created
*/
LinkedList* alloc_linkedList();

/**
* advancing the current to the next node.
* @pre list != NULL
* @param list
*/
void advance_curr(LinkedList *list);

/**
* moves the current pointer to the previous node.
* @pre list != NULL
* @param list
*/
void backward_curr(LinkedList *list);
/**
* @param list != NULL
* @return 1 if succeeded, otherwise the number of error
*/
int remove_next_after_curr(LinkedList *list);

/**
* @pre node is in list
* @param list != NULL. list of nodes type Node
* @param node
* removes input node from input list, and frees it.
* notice - if node == list.current: new current = node.prev
* if also node == list.head: new current = node.next
*/
void remove_node(LinkedList *list, Node *node);

/**
* removes all the nodes after current.
* @pre list != NULL
* @param list
*/
void remove_all_after_curr(LinkedList *list);

/**
* frees the memory allocated for node
* @param node - node to free
*/
void free_node(Node *node);

/**
* frees all the memory allocated for the list and its nodes
* @param list
*/
void free_linked_list(LinkedList* list);

/*TODO: Or: delete later*/
void print_linked_list_prevmode(LinkedList* list);
#endif