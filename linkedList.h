
#ifndef SP_LINKEDLIST_H
#define SP_LINKEDLIST_H

#include <stdlib.h>
#include "linkedListCells.h"
#include "command.h"

/**
 *the linked list DS to support undo/redo
 */

/**
* struct of node contains a command, two list of cells: one for the cells values before the command execution
* and one for the cells values after the command execution. In addition, a node contains a pointer to the previous node
* and a pointer to the next node.
*/
typedef struct NodeLinkedList{
    Command* c;
    LinkedListCells* old_values;
    LinkedListCells* new_values;
    struct NodeLinkedList* next;
    struct NodeLinkedList* prev;
}Node;

/**
* struct of list contains nodes as described above. each list has pointer to it's head node, current node
* and a field for length of the list
*/
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

/**
* @param node != NULL
* @return the linked list of cells old values of input node
*/
LinkedListCells * get_old_values_cells_list(Node *node);
/**
* @param node != NULL
* @return the linked list of cells new values of input node
*/
LinkedListCells * get_new_values_cells_list(Node *node);


/**
* @param list != NULL
* @return 1 if current points to last node of the list. 0 otherwise (means there is a node after current)
*/
int is_curr_last(LinkedList* list);

/**
* @param node != NULL
* @return a pointer to the command of input node
*/
Command* get_command(Node* node);

/**
* adds after current node a new node with input arguments as properties.
* if current == NULL (the list is empty) than node will become current and head.
* @param list of nodes from Node type
* @param c - the executed command
* @param old_values - a linked list of the cells which had been changed due to the command execution
*/
void add_linked_list(LinkedList *list, Command *c, LinkedListCells *old_values, LinkedListCells *new_values);

/**
* allocates memory on the heap for node type.
* @param c - the executed command
* @param old_values - a linked list of the cells which had been changed due to the command execution
* @return pointer to a new node with input arguments as properties. next and prev are both NULL at that point
*/
Node *alloc_node(Command *c, LinkedListCells *old_values, LinkedListCells *new_values);
/**
* allocates memory on the heap for linked list type.
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
* removes all the nodes after current.
* @pre list != NULL
* @param list
*/
void remove_all_after_curr(LinkedList *list);

/**
* frees the memory allocated for node. NOTICE: the next and prev are not freed
* @param node - node to free
*/
void free_node(Node *node);

/**
* frees all the memory allocated for the list and its nodes
* @param list
*/
void free_linked_list(LinkedList* list);


/**
* remove all nodes of the list but the head node. At the end, current node points to head node.
* @param list != NULL
*/
void clear_linked_list(LinkedList* list);
#endif