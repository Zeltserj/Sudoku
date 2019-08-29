
#ifndef SP_STACK_H
#define SP_STACK_H

#include <stdlib.h>
#include "errors.h"

/**
* struct of stack element referred as node. Each contains properties of a cell: row, column, value, array of possible
* solutions for the cell, and a pointer to next element of the stack.
*/
typedef struct StackNode{
    int row, col, value;
    int* possible_sols;
    struct StackNode* next;
}StackNode;
/**
* struct of stack - LIFO list. Contains a poinet to the head of the stack and field for its size (number of elements)
*/
typedef struct Stack{
    StackNode* head;
    int size;
}Stack;

/**
* allocates memory for a stack on the heap.
* @return a pointer to the stack allocated.
*/
Stack * alloc_stack();

/**
* allocates memory for a stack element referred as node. The memory allocated on the heap.
* @param row >= 0
* @param col >= 0
* @param value >= 0
* @return a pointer to the new node of stack contains the input parameters as properties.
*/
StackNode *alloc_stack_node(int row, int col, int value);

/**
* adds a new element to the stack, contains the input parameters as properties.
* At the end of the method, stack head element is that new element.
* @param stack != NULL
* @param row >= 0
* @param col >= 0
* @param value >= 0
*/
void push(Stack *stack, int row, int col, int value);
/**
* removes the top element of the stack and returns it
* @param stack != NULL
* @return the top element of the stack.if stack is empty, returns NULL
*/
StackNode* pop(Stack *stack);
/**
*
* @param stack != NULL
* @return the top element of the stack (does not removes it). if stack is empty, returns NULL
*/
StackNode* peek(Stack *stack);

/**
* @param stack != NULL
* @return 1 if the stack is empty. otherwise 0
*/
int is_empty_stack(Stack *stack);

/**
* @param stack != NULL
* @return the number of elements in the stack
*/
int size_stack(Stack* stack);

/**
* frees the node fields. NOTICE: doesn't free the next node that node has a pointer to.
* @param node - a pointer to a stack element
*/
void free_stack_node(StackNode *node);
/**
* frees all memory of stack
* @param stack - a pointer to a stack
*/
void free_stack(Stack* stack);

/**
* sets sols_arr as possible solutions field of input node.
* @param node != NULL
* @param sols_arr - array of possible solutions to the cell represented by input node fields (row & column)
*/
void set_possible_sols(StackNode *node, int *sols_arr);
/**
* sets input value as value field of input node.
* @param node != NULL
* @param value - value to the cell represented by input node fields (row & column)
*/
void set_value_stack_node(StackNode *node, int value);

/**
* @param node != NULL
* @return row of cell represented by node
*/
int get_row_stack_node(StackNode *node);
/**
* @param node != NULL
* @return column of cell represented by node
*/
int get_col_stack_node(StackNode *node);
/**
* @param node != NULL
* @return value of cell represented by node
*/
int get_value_stack_node(StackNode *node);

/**
* @param node != NULL
* @return a pointer to an array of possible solutions of the cell represented by node
*/
int* get_possible_sols(StackNode* node);

#endif /*SP_STACK_H*/
