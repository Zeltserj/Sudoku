
#include "execute.h"
#include "game.h"

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


