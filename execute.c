
#include <stdio.h>
#include "execute.h"
#include "game.h"
#include "solver.h"
#include "cell.h"
#include "fileaux.h"
#include "command.h"

void increase_conflicts(Cell **cells);
void decrease_conflicts(Cell **cells);

int execute_command(Board *board, Command *command, LinkedList *moves) {
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
    if (get_curr(moves) == get_head(moves)) {
        return 0;
    } else {
        temp = get_curr(moves);
        backward_curr(moves);
        change_cells_to(board, get_changed_cells_list(temp));
        return 1;
    }
}
void change_cells_to(Board *board, LinkedListCells *old_values) {
    int i;

    move_curr_to_head(old_values);
    for(i=0;i<get_len(old_values);i++){
        set_cell(board,get_curr_cell(old_values));
        advance_curr_cell(old_values);
    }
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

int set_command(Board *board, LinkedList *moves, int r, int c, int value) {
    Cell** cell_arr = illegal_neighbours(board,r,c,value);
    LinkedListCells* curr_changed, curr;
    Node* node;
    int i, err=0;
    for(i=0; i<3;i++){
        if(cell_arr[i] != NULL){
            set_erroneous(board,get_cell_row(cell_arr[i]),get_cell_col(cell_arr[i]));
            node = get_curr(moves);
            curr_changed = get_changed_cells_list(node);
            printf("after curr_changed\n");
            if(get_cell_error(cell_arr[i])) {
                add_cell_after_curr(curr_changed, cell_arr[i]);
            }
            err=1;
        }
    }
    if(err)
        set_erroneous(board,r,c);
    free(cell_arr);
    set_value(board,r,c,value);
}

void increase_conflicts(Cell **cells) {

}

void decrease_conflicts(Cell **cells) {

}


