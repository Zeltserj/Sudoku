
#include <stdio.h>
#include "execute.h"
#include "game.h"
#include "solver.h"
#include "cell.h"
#include "fileaux.h"
#include "command.h"

/*TODO: Or: the first node in moves has command==NULL therefore moves is not empty*/
int execute_command(Board *board, Command *command, LinkedList *moves) {
    LinkedListCells* changed = alloc_linked_list_cells();
    if(command->type == SET || command->type == GENERATE || command->type == GUESS || command->type == AUTOFILL) {
        printf("execute command before adding to moves.\n");
        add_linked_list(moves, command, changed,mode++);
        printf("execute command after adding to moves\n");
        advance_curr(moves);
    }
    else
        free(changed);

    switch (command->type) {
        case SOLVE:
            board = load(get_filepath(command));
            print_board(board);
            set_mode(1);
            break;
        case EDIT:
            board = load(get_filepath(command));
            print_board(board);
            set_mode(2);
            break;
        case MARK_ERRORS:
            mark_errors_command(get_parameters(command)[0]);
            break;
        case PRINT_BOARD:
            print_board(board);
            break;
        case SET:
            set_command(board,moves,get_parameters(command)[0],get_parameters(command)[1],get_parameters(command)[2]);
            print_board(board);
            break;
        case VALIDATE:
            break;
        case GUESS:
            break;
        case GENERATE:
            break;
        case UNDO:
            if (undo(board, moves))
                print_board(board);
            else
                command_error(8);
            break;
        case REDO:
            if(!redo(board,moves))
                command_error(9);
            break;
        case SAVE:
            save(board,get_filepath(command));
            break;
        case HINT:
            break;
        case GUESS_HINT:
            break;
        case NUM_SOLUTIONS:
            break;
        case AUTOFILL:
            autofill_command(board, moves);
            print_board(board);
            break;
        case RESET:
            reset_command(board,moves);
            print_board(board);
            break;
        case EXIT:
            exit_command(board,moves);
            print_exit_command();
            break;
    }
}

void set_mode(int new_mode) {
     mode = new_mode;
}

void mark_errors_command(int mark) {
    mark_errors = mark;
}

int undo(Board *board, LinkedList *moves) {
    Node *temp;
    Node* curr = get_curr(moves);
    if ( get_command(curr) == NULL) {
        return 0;
    } else {
        temp = curr;
        backward_curr(moves);
        change_cells_to(board, get_changed_cells_list(temp));
        return 1;
    }
}
void change_cells_to(Board *board, LinkedListCells *old_values) {
    int i, len = get_len_linked_list_cells(old_values);
    Cell* curr;
    move_curr_to_head(old_values);
    for(i=0; i < len; i++){
        curr=get_curr_cell(old_values);
        set_cell(board,curr);
        advance_curr_cell(old_values);
    }
    move_curr_to_head(old_values);
}

int redo(Board *board, LinkedList *moves) {
    Command* c;
    if (is_curr_last(moves)){
        return 0;
    }
    advance_curr(moves);
    c = get_command(get_curr(moves));
    /*c is one of: set/autofill/generate/guess */
    execute_command(board,c,moves); /*TODO: Or: might changed later to switch and specific methods*/
    return 1;

}

void autofill_command(Board *board, LinkedList *moves) {
    Board* b_cpy = brdcpy(board);
    int i,j,v;
    for(i=0; i<get_size(board);i++){
        for(j=0;j<get_size(board);j++){
            if(get(b_cpy,i,j) == 0) {
                v = get_single_value(b_cpy, i, j);
                if (v != 0) {
                    /*TODO: Or: make sure cells marked as errors if needed*/
                    set_command(board, moves, i, j, v);
                }
            }
        }
    }
    free_board(b_cpy);
}
int get_single_value(Board* board,int r, int c){
    int* sols = get_all_sol_cell(board,r,c);
    int single_sol=0, i;

    for(i=0;i<get_size(board);i++){
        if(sols[i]==1){
            if(single_sol != 0){
                free(sols);
                return 0;
            }
            single_sol = i+1;
        }
    }
    free(sols);
    return single_sol;
}

void reset_command(Board *board, LinkedList *moves) {
    while (undo(board,moves)){}
}

void exit_command(Board *board, LinkedList *moves) {
    free_board(board);
    free_linked_list(moves);
}

void print_exit_command() {
    printf("Exiting...\n");
}

void set_command(Board *board, LinkedList *moves, int r, int c, int value) {
    Node* curr = get_curr(moves);
    LinkedListCells* curr_changed = get_changed_cells_list(curr);
    int prev_value=get(board,r,c);
    if(value!= prev_value)
    {
        add_cell_after_curr(curr_changed,get_cell_cpy(board,r,c));
        if(is_error(board,r,c))
            validate_cell(board, curr_changed, r, c, prev_value,-1);
        validate_cell(board,curr_changed,r,c,value,1);
        set_value(board,r,c,value);
    }
    printf("set_command: linked list of command:\n");
    print_linked_list_cells(curr_changed);
    printf("done set_command\n");
}

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
