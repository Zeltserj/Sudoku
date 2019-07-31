
#include "execute.h"
#include "game.h"

int execute_command(Command *command) {
    switch (command->type){
        case SOLVE:break;
        case EDIT:break;
        case MARK_ERRORS:break;
        case PRINT_BOARD:break;
        case SET:break;
        case VALIDATE:break;
        case GUESS:break;
        case GENERATE:break;
        case UNDO:

            break;
        case REDO:
            break;
        case SAVE:break;
        case HINT:break;
        case GUESS_HINT:break;
        case NUM_SOLUTIONS:break;
        case AUTOFILL:break;
        case RESET:break;
        case EXIT:break;
    }
}

void set_mode(int new_mode) {
     mode = new_mode;
}

void mark_errors_command(int mark) {
    mark_errors = mark;
}

void undo(Board *board, LinkedList *moves) {
    Node* temp = moves->current;
    LinkedListCells* cells = temp->changed;
    backward_curr(moves);

}
void change_cells_to(Board *board, LinkedListCells *old_values) {
    int i;
    move_curr_to_head(old_values);
    for(i=0;i<old_values->len;i++){
        cell_copy(board,old_values->current);
        advance_curr_cell(old_values);
    }
}
