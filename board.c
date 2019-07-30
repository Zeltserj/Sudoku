
#include <stdio.h>
#include <stdlib.h>
#include "board.h"


void free_row(Cell **cell_row, int size);

Cell ***matrix_copy(Cell ***matrix, int size);

void cell_copy(Cell *to, Cell *from);

void set_cell_value(Cell *cell, int value){ cell->value = value;}
void set_cell_fixed(Cell *cell, int fixed){cell->fixed = fixed;}
void set_cell_error(Cell *cell, int is_error) {cell->error = is_error;}

int get_cell_value(Cell *cell) {return cell->value;}
int get_cell_fixed(Cell *cell) {return cell->fixed;}
int get_cell_error(Cell *cell) {return cell->error;}
int get_cell_row(Cell *cell) {return cell->row;}
int get_cell_col(Cell *cell) {return cell->col;
}

void set_cell_location(Cell *cell, int r, int c) {
    cell->row = r;
    cell->col = c;

}

Cell *alloc_cell(int r, int c) {
    Cell* cell = (Cell*) calloc(1, sizeof(Cell));
    if(cell == NULL){
        error("board", "alloc_cell", 1); /*memory allocation failure*/
        exit(0);
    }
    set_cell_location(cell,r,c);
    return cell;
}


Cell *** alloc_matrix(int size) {
    int i,j;
    Cell ***matrix = (Cell***)calloc(size, sizeof(Cell**));
    if(matrix == NULL){
        error("board", "alloc_matrix", 1);
        exit(0);
    }
    for(i = 0; i < size; i ++){
        matrix[i] = (Cell**)calloc(size, sizeof(Cell*));
        if(matrix[i] == NULL){
            for(j = 0; j < i; j ++){
                free_row(matrix[j], size);
                free(matrix);
            }
        }
        for(j = 0; j < size; j++){
            matrix[i][j] = (Cell*)calloc(1, sizeof(Cell));
            if(matrix[i][j] == NULL) {
                free_row(matrix[i], j);

                for (j = 0; j < i; j++) {
                    free_row(matrix[j], size);
                }
                free(matrix);
                error("board", "alloc_matrix", 1);
                exit(0);
            }

        }
    }
    return matrix;
}

Board *alloc_board(int r, int c) {

    Board *board = (Board*) calloc(1, sizeof(Board));
    if(board == NULL){
        error("board", "alloc_board", 1);
        exit(0);
    }


    board->rows = r;
    board->cols = c;
    board->size = r*c;
    board->num_empty = r*c*r*c;
    board->matrix = alloc_matrix(board->size);
    if(board->matrix == NULL){
        free(board);
        error("board", "alloc_board", 1);
        exit(0);
    }

    return board;
}

void free_row(Cell **cell_row, int size) {
    int i;
    for(i=0;i<size;i++){
        free(cell_row[i]);
    }
    free(cell_row);
}

void set(Board *board, int r, int c, int value) {
    set_cell_value(board->matrix[r][c], value);

}

int get(Board *board, int r, int c) {
    return get_cell_value(board->matrix[r][c]);
}

int is_fixed(Board *board, int r, int c) {
    return get_cell_fixed(board->matrix[r][c]);
}

int is_error(Board *board, int r, int c) {
    return get_cell_error(board->matrix[r][c]);
}

void free_matrix(Cell ***matrix, int size) {
    int i;
    for(i=0; i < size; i++){
        free_row(matrix[i], size);
    }
    free(matrix);
}

void free_board(Board *board) {
    free_matrix(board->matrix,board->size);
    free(board);

}

Board *brdcpy(Board *board) {
    Board* out = alloc_board(board->rows, board->cols);
    if(out == NULL){
        error("board", "brdcpy", 1);
        exit(0);
    }
    out->matrix = matrix_copy(board->matrix, board->size);
    out->num_empty = board->num_empty;
    return out;
}

void fix_cell(Board *board, int r, int c) {
    set_cell_fixed(board->matrix[r][c], 1);

}

void unfix_cell(Board *board, int r, int c) {
    set_cell_fixed(board->matrix[r][c], 0);
}

void set_erroneous(Board *board, int r, int c) {
    set_cell_error(board->matrix[r][c], 1);

}

void set_legal(Board *board, int r, int c) {
    set_cell_error(board->matrix[r][c], 0);
}


Cell*** matrix_copy(Cell*** matrix, int size){
    int i, j;
    Cell*** out = alloc_matrix(size);
    if(out == NULL){
        error(NULL, NULL, 10501);
        return NULL;
    }
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            cell_copy(out[i][j], matrix[i][j]);
        }
    }
    return out;
}

void cell_copy(Cell *to, Cell *from) {
    to->col = from->col;
    to->row = from->row;
    to->error = from->error;
    to->fixed = from->fixed;
    to->value = from->value;
}




