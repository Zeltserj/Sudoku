
#include "execute.h"
#include "game.h"
#include "solver.h"

int execute_command(Board *board, Command *command, LinkedList *moves) {
    switch (command->type) {
        case SOLVE:
            break;
        case EDIT:
            break;
        case MARK_ERRORS:
            break;
        case PRINT_BOARD:
            print_board(board);
            break;
        case SET:
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
            break;
        case HINT:
            break;
        case GUESS_HINT:
            break;
        case NUM_SOLUTIONS:
            break;
        case AUTOFILL:
            autofill_command(board);
            print_board(board);
            break;
        case RESET:
            break;
        case EXIT:
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
    if (moves->current == moves->head) {
        return 0;
    } else {
        temp = moves->current;
        backward_curr(moves);
        change_cells_to(board, temp->changed);
        return 1;
    }
}
void change_cells_to(Board *board, LinkedListCells *old_values) {
    int i;
    move_curr_to_head(old_values);
    for(i=0;i<old_values->len;i++){
        set_cell(board,old_values->current->c);
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

void autofill_command(Board *board) {
    Board* b_cpy = brdcpy(board);
    int i,j,v;
    for(i=0; i<board->size;i++){
        for(j=0;j<board->size;j++){
            if(get(b_cpy,i,j) == 0) {
                v = get_single_value(b_cpy, i, j);
                if (v != 0) {
                    /*TODO: Or: make sure cells marked as errors if needed*/
                    set_command(board, i, j, v);
                }
            }
        }
    }
    free_board(b_cpy);
}
int get_single_value(Board* board,int r, int c){
    int* sols = get_all_sol_cell(board,r,c);
    int single_sol=0, i;

    for(i=0;i<board->size;i++){
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

