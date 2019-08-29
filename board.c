
#include <stdio.h>
#include <stdlib.h>
#include "board.h"


void free_row(Cell **cell_row, int size);

Cell ***matrix_copy(Cell ***matrix, int size);


void print_dashes(int length);

Cell ***alloc_matrix(int size) {
    int i, j;
    Cell ***matrix = (Cell ***) calloc(size, sizeof(Cell **));
    if (matrix == NULL) {
        error("board", "alloc_matrix", 1);
        exit(0);
    }
    for (i = 0; i < size; i++) {
        matrix[i] = (Cell **) calloc(size, sizeof(Cell *));
        if (matrix[i] == NULL) {
            for (j = 0; j < i; j++) {
                free_row(matrix[j], size);
                free(matrix);
            }
        }
        for (j = 0; j < size; j++) {
            matrix[i][j] = (Cell *) calloc(1, sizeof(Cell));
            if (matrix[i][j] == NULL) {
                free_row(matrix[i], j);

                for (j = 0; j < i; j++) {
                    free_row(matrix[j], size);
                }
                free(matrix);
                error("board", "alloc_matrix", 1);
                exit(0);
            }
            set_cell_location(matrix[i][j],i,j);
        }
    }
    return matrix;
}

Board *alloc_board(int r, int c) {

    Board *board = (Board *) calloc(1, sizeof(Board));
    if (board == NULL) {
        error("board", "alloc_board", 1);
        exit(0);
    }


    board->rows_block = r;
    board->cols_block = c;
    board->size = r * c;
    board->num_empty = r * c * r * c;
    board->matrix = alloc_matrix(board->size);
    if (board->matrix == NULL) {
        free(board);
        error("board", "alloc_board", 1);
        exit(0);
    }

    return board;
}

void free_row(Cell **cell_row, int size) {
    int i;
    if(cell_row != NULL){
        for (i = 0; i < size; i++) {
            free(cell_row[i]);
        }
        free(cell_row);
    }
}

void set_value(Board *board, int r, int c, int value) {
    int prev_value = get(board, r, c);
    if (prev_value != 0 && value == 0)
        board->num_empty++;
    else if (prev_value == 0 && value != 0)
        board->num_empty--;
    set_cell_value(board->matrix[r][c], value);
}

int get(Board *board, int r, int c) {
    return get_cell_value(board->matrix[r][c]);
}


int is_fixed(Board *board, int r, int c) {
    return get_cell_fixed(board->matrix[r][c]);
}

int is_error(Board *board, int r, int c) {
    if(get_cell_error(board->matrix[r][c])>0){
        return 1;
    }
    return 0;
}

void free_matrix(Cell ***matrix, int size) {
    int i;
    if(matrix != NULL){
        for (i = 0; i < size; i++) {
            free_row(matrix[i], size);
        }
        free(matrix);
    }
}

void free_board(Board *board) {
    if(board!=NULL){
        free_matrix(board->matrix, board->size);
        free(board);
    }
}

Board *brdcpy(Board *board) {
    Board *out = alloc_board(board->rows_block, board->cols_block);
    if (out == NULL) {
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

void print_board(Board *board) {
    int row_length = (board->size * 4) + board->rows_block + 1;
    int i, j;
    int cell;
    for (i = 0; i < board->size; i++) {
        if ((i % board->rows_block) == 0) {
            print_dashes(row_length);
        }
        for (j = 0; j < board->size; j++) {
            if (j % board->cols_block == 0) {
                printf("|");
            }
            cell = get(board,i,j);
            if(cell == 0){
                printf("    ");
            }
            else if(is_fixed(board,i,j)){
                printf(" %2d.", cell);
            }
            else if((mode == 2 || mark_errors) && is_error(board, i , j)){
                printf(" %2d*", cell);
            }
            else{
                printf(" %2d ", cell);
            }
        }
        printf("|\n");
    }
    print_dashes(row_length);

}

void print_dashes(int length) {
    int i;
    for (i = 0; i < length; i++) {
        printf("-");
    }
    printf("\n");

}


Cell ***matrix_copy(Cell ***matrix, int size) {
    int i, j;
    Cell ***out = alloc_matrix(size);
    /*if (out == NULL) {
        error(NULL, NULL, 10501);
        return NULL;
    }*/
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            cell_copy(out[i][j], matrix[i][j]);
        }
    }
    return out;
}

void set_cell(Board *board, Cell *cell) {
    int r = get_cell_row(cell);
    int c = get_cell_col(cell);
    set_cell_fixed(board->matrix[r][c],get_cell_fixed(cell));
    set_cell_error(board->matrix[r][c],get_cell_error(cell));
    /*printf("set_cell: curr - [%d][%d], error = %d",r,c,cell->error );*/
    set_value(board,r,c,get_cell_value(cell));
}




int get_size(Board *board) {
    return board->size;
}

int is_erroneous(Board *board) {
    int i, j;
    for(i = 0; i < get_size(board); i++){
        for(j = 0 ; j < get_size(board); j++){
            if(is_error(board,i,j)){
                return 1;
            }
        }
    }
    return 0;
}

int get_num_empty(Board *board) {
    return board->num_empty;
}

int increase_error(Board *board, int r, int c) {
    Cell* cell =board->matrix[r][c];
    increase_cell_error(cell);
    if(get_cell_error(cell)==1)
        return 1;
    return 0;
}

int decrease_error(Board *board, int r, int c) {
    Cell* cell =board->matrix[r][c];
    decrease_cell_error(cell);
    if(get_cell_error(cell)==0)
        return 1;
    return 0;
}

Cell *get_cell_cpy(Board *board, int r, int c) {
    Cell* cpy = alloc_cell(r,c);
    cell_copy(cpy,board->matrix[r][c]);
    return cpy;
}

int get_block_rows(Board *board) { return board->rows_block; }
int get_block_cols(Board *board) { return board->cols_block; }











