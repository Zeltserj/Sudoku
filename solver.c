
#include "solver.h"
#include <stdlib.h>
#include <stdio.h>

/*TODO: Or: moved  get_all_sol_cell to execute, to avoid include*/

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its row
*/
int validate_cell_row(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int i, num_errors=0,is_changed=0;
    Cell* cell_cpy;
    for(i=0;i<get_size(board);i++){
        if(i!=c && get(board,r,i) == value) {
            if(!is_fixed(board,r,i)){
                cell_cpy=get_cell_cpy(board,r,i);
                if(mark_errors == 1)
                    is_changed = increase_error(board,r,i);
                else if(mark_errors == -1)
                    is_changed = decrease_error(board,r,i);

                if(mark_errors != 0 && is_changed)
                    add_cell_after_curr(changed_cells,cell_cpy);
                else
                    free(cell_cpy);
            }
            num_errors++;
        }
    }
    return num_errors;
}
/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its column
*/
int validate_cell_col(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int i, num_errors=0, is_changed=0;
    Cell* cell_cpy;
    for(i=0;i<get_size(board);i++){
        if(i!=r && get(board,i,c) == value){
            if(!is_fixed(board,i,c)){
                cell_cpy=get_cell_cpy(board,i,c);
                if(mark_errors == 1)
                    is_changed=increase_error(board,i,c);
                else if(mark_errors == -1)
                    is_changed=decrease_error(board,i,c);

                if(mark_errors != 0 && is_changed)
                    add_cell_after_curr(changed_cells,cell_cpy);
                else
                    free(cell_cpy);
            }
            num_errors++;
        }
    }
    return num_errors;
}
/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its block.
*/
int validate_cell_block(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int first_r, first_c, i,j, num_errors = 0,is_changed=0;
    int block_r = get_block_rows(board);
    int block_c = get_block_cols(board);
    Cell* cell_cpy;

    first_r = (r/block_r)*block_r;
    first_c = (c/block_c)*block_c;
    for(i=first_r;i<first_r+block_r;i++){
        for(j=first_c;j<first_c+block_c;j++){
            if((i!=r && j!=c) && get(board,i,j)==value){
                if(!is_fixed(board,i,j)) {
                    cell_cpy = get_cell_cpy(board, i, j);
                    if (mark_errors == 1)
                        is_changed = increase_error(board, i, j);
                    else if (mark_errors == -1)
                        is_changed = decrease_error(board, i, j);

                    if (mark_errors != 0 &&
                        is_changed) { /*row & col conflicts are handled by validate_cell_row/cell. */
                        add_cell_after_curr(changed_cells, cell_cpy);
                    } else
                        free(cell_cpy);
                }
                num_errors++;
            }
        }
    }
    return num_errors;
}

int validate_cell(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int num_errors, is_valid = 0;
    int num_err_row, num_err_col, num_err_block;
    num_err_row = validate_cell_row(board, changed_cells, r, c, value, mark_errors);
    num_err_col = validate_cell_col(board, changed_cells, r, c, value, mark_errors);
    num_err_block = validate_cell_block(board, changed_cells, r, c, value, mark_errors);
    num_errors = num_err_row + num_err_col + num_err_block;
    if (num_errors == 0)
        is_valid = 1;
    if (mark_errors != 0) {
        while (num_errors != 0) {
            if (mark_errors > 0)
                increase_error(board, r, c);
            else
                decrease_error(board, r, c);
            num_errors--;
        }
    }
    return is_valid;
}

int *get_all_sol_cell(Board *board, int r, int c) {
    int* sol = calloc(get_size(board), sizeof(int));
    int i, num_sol=0;
    if(sol==NULL){
        error("execute","get_all_sol_cell",1);
        exit(0);
    }
    for(i=0;i<get_size(board);i++){
        if(validate_cell(board,NULL, r, c, i + 1,0)){
            sol[i] = 1;
            num_sol++;
        }
    }
    if(num_sol==0)
        return NULL;
    return sol;
}
