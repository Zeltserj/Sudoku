
#ifndef SP_STACK_H
#define SP_STACK_H

#include <stdlib.h>
#include "errors.h"

typedef struct StackNode{
    int row, col, value;
    struct StackNode* next;
}StackNode;

typedef struct Stack{
    StackNode* head;
}Stack;

Stack * alloc_stack();
StackNode *alloc_stack_node(int row, int col, int value);

void push(Stack *stack, StackNode *node);
StackNode* pop(Stack *stack);
StackNode* peek(Stack *stack);

int is_empty_stack(Stack *stack);

void free_stack_node(StackNode *node);
void free_stack(Stack* stack);

#endif /*SP_STACK_H*/
