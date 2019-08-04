
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
    for (i = 0; i < size; i++) {
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
    for (i = 0; i < size; i++) {
        free_row(matrix[i], size);
    }
    free(matrix);
}

void free_board(Board *board) {
    free_matrix(board->matrix, board->size);
    free(board);

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

void set_erroneous(Board *board, int r, int c) {
    set_cell_error(board->matrix[r][c], 1);

}

void set_legal(Board *board, int r, int c) {
    set_cell_error(board->matrix[r][c], 0);
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
            else if(mode == 2 || (mark_errors && is_error(board, i , j))){
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
    if (out == NULL) {
        error(NULL, NULL, 10501);
        return NULL;
    }
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            cell_copy(out[i][j], matrix[i][j]);
        }
    }
    return out;
}

/*TODO: let's figure out what's the best way to avoid redundant functions*/
void set_cell(Board *board, Cell *cell) {
    int r = get_cell_row(cell);
    int c = get_cell_col(cell);
    Cell* b_cell = board->matrix[r][c];
    set_cell_error(b_cell,cell->error);
    set_cell_fixed(b_cell,cell->fixed);
    set_cell_value(b_cell,cell->value);
}

/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @return NULL if value does not exist in row r on board (exclude cell board[r][c]).
* otherwise, return the cell with same value.
*/
Cell * is_legal_row(Board *board, int r, int c, int value) {
    int i;
    for(i=0;i<board->size;i++){
        if(i!=c && get(board,r,i) == value) {
            return board->matrix[r][i];
        }
    }
    return NULL;
}
/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @return NULL if value does not exist in column r on board (exclude cell board[r][c]).
* otherwise, return the cell with same value.
*/
Cell * is_legal_col(Board *board, int r, int c, int value) {
    int i;
    for(i=0;i<board->size;i++){
        if(i!=r && get(board,i,c) == value){
            return board->matrix[i][c];
        }
    }
    return NULL;
}
/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @return 1 if value does not exist in cell's (board[r][c]) block on board (exclude that cell).
* 0 otherwise.
*/
Cell * is_legal_block(Board *board, int r, int c, int value) {
    int first_r, first_c;
    int i,j;
    first_r = (r/board->rows_block)*board->rows_block;
    first_c = (c/board->cols_block)*board->cols_block;

    for(i=first_r;i<first_r+board->rows_block;i++){
        for(j=first_c;j<first_c+board->cols_block;j++){
            if((i!=r || j!=c) && get(board,i,j)==value ){
                return board->matrix[i][j];
            }
        }
    }
    return NULL;
}

Cell ** is_legal_value(Board *board, int r, int c, int value) {
    Cell** cell_arr = calloc(3, sizeof(Cell*));
    cell_arr[0] = is_legal_row(board,r,c,value);
    cell_arr[1]=is_legal_col(board,r,c,value);
    cell_arr[2]=is_legal_block(board,r,c,value);
    return cell_arr;
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








