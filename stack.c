
#include "stack.h"


Stack * alloc_stack() {
    Stack* newSrack = calloc(1,sizeof(int*));
    if(newSrack==NULL){
        error("stack","alloc_stack",1);
        exit(0);
    }
}

StackNode *alloc_stack_node(int row, int col, int value) {
    StackNode* node = calloc(4, sizeof(int*));
    if(node == NULL){
        error("stack","alloc_stack_node",1);
        exit(0);
    }
    node->row=row;
    node->col=col;
    node->value=value;
}

void push(Stack *stack, StackNode *node) {
    StackNode* temp= stack->head;
    stack->head = node;
    node->next = temp;
}

StackNode *pop(Stack *stack) {
    StackNode* temp=stack->head;
    stack->head=stack->head->next;
    return temp;
}

StackNode *peek(Stack *stack) {
    return stack->head;
}

int is_empty_stack(Stack *stack) {
    return (stack->head==NULL);
}

void free_stack_node(StackNode *node) {
    free(node->next);
    free(node);
}

void free_stack(Stack* stack) {
    StackNode* temp = peek(stack);
    while(temp!=NULL && !is_empty_stack(stack)){
        temp = pop(stack);
        free(temp);
    }
    free(stack);
}



