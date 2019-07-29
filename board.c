/**

 Created by lenovo on ${DTE}
 **/


#include <stdio.h>
#include <stdlib.h>
#include "board.h"


Cell *alloc_cell() {
    Cell* cell = (Cell*) calloc(1, sizeof(Cell));
    if(cell == NULL){
        error(10101); /*memory allocation failure*/
        return NULL;
    }
    return cell;
}

Board *alloc_board(int r, int c, int value) {
    Board *board = (Board*) calloc(1, sizeof(Board));
    if(board == NULL){
        error(10201);
    }
    board->rows = r;
    board->cols = c;
    board->size = r*c;
    board->num_empty = r*c*r*c;
    board->matrix = alloc_matrix(r,c);
    if(board->matrix == NULL){
        free(board);
        error(10201);
    }

    return board;
}

Cell **alloc_matrix(int size) {
    int i,j;
    Cell **matrix = (Cell **) calloc(size, sizeof(Cell *));
    if(matrix == NULL){
        error(10301);
    }
    for(i=0; i < size; i++){
        matrix[i] = (Cell*)calloc(size, sizeof(Cell));
        if(matrix[i] == NULL){
            for(j = 0; j < i; j++){
                free_cell(matrix[j]);
            }
            free(matrix);
            error(10301);
        }


    }
    return matrix;
}


void set_cell_value(Cell *cell, int value) {
    cell->value = value;
}

void set(Board *board, int r, int c, int value) {
    set_cell_value(&board->matrix[r][c], value);

}
