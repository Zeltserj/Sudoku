
#include "stack.h"

int size_stack(Stack *stack) {
    return stack->size;
}
Stack * alloc_stack() {
    Stack* newStack = (Stack*)calloc(1, sizeof(Stack));
    if(newStack == NULL){
        error("stack","alloc_stack",1);
        exit(0);
    }
    return newStack;
}

StackNode *alloc_stack_node(int row, int col, int value) {
    StackNode* node = (StackNode*) calloc(1, sizeof(StackNode));
    if(node == NULL){
        error("stack","alloc_stack_node",1);
        exit(0);
    }
    /*node->possible_sols = calloc(board_size, sizeof(int));
    if(node->possible_sols == NULL){
        error("stack","alloc_stack_node",1);
        exit(0);
    }*/
    node->possible_sols=NULL;
    node->row=row;
    node->col=col;
    node->value=value;
    return node;
}

void push(Stack *stack, int row, int col, int value) {
    StackNode* new_node = alloc_stack_node(row, col, value);
    StackNode* temp;
    if(size_stack(stack) == 0){
        stack->head = new_node;
    }
    else{
        temp= stack->head;
        stack->head = new_node;
        new_node->next = temp;
    }
    stack->size++;
}

StackNode *pop(Stack *stack) {
    StackNode* temp=stack->head;
    if(stack->size>0) {
        stack->head = stack->head->next;
        stack->size--;
    }
    return temp;
}

StackNode *peek(Stack *stack) {
    return stack->head;
}

int is_empty_stack(Stack *stack) {
    if(stack->size==0)
        return 1;
    return 0;
}

void free_stack_node(StackNode *node) {
    free(node->possible_sols);
    free(node);
}

void free_stack(Stack* stack) {
    StackNode* temp;
    if(stack!=NULL && !is_empty_stack(stack)) {
        temp = pop(stack);
        while (temp!= NULL && !is_empty_stack(stack)) {
            temp = pop(stack);
            free(temp);
        }
    }
    free(stack);
}


int get_row_stack_node(StackNode *node) { return node->row; }

int get_col_stack_node(StackNode *node) { return node->col; }

int get_value_stack_node(StackNode *node){ return node->value; }

int *get_possible_sols(StackNode *node) { return node->possible_sols; }

void set_possible_sols(StackNode *node, int *sols_arr) { node->possible_sols = sols_arr; }

void set_value_stack_node(StackNode *node, int value) { node->value = value; }





