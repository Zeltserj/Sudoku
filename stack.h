
#ifndef SP_STACK_H
#define SP_STACK_H

#include <stdlib.h>
#include "errors.h"

typedef struct StackNode{
    int row, col, value;
    int* possible_sols;
    struct StackNode* next;
}StackNode;

typedef struct Stack{
    StackNode* head;
    int size;
}Stack;

Stack * alloc_stack();
StackNode *alloc_stack_node(int row, int col, int value);

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

void free_stack_node(StackNode *node);
void free_stack(Stack* stack);

void set_possible_sols(StackNode *node, int *sols_arr);
void set_value_stack_node(StackNode *node, int value);
int get_row_stack_node(StackNode *node);
int get_col_stack_node(StackNode *node);
int get_value_stack_node(StackNode *node);
int* get_possible_sols(StackNode* node);
#endif /*SP_STACK_H*/
