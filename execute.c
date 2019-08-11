
#include <stdio.h>
#include "execute.h"
#include "boardModifier.h"
#include "fileaux.h"


/*TODO: Or: the first node in moves has command==NULL therefore moves is not empty*/
int execute(Board **game_board, Command *command, LinkedList **game_moves){
    return execute_command(game_board,command,game_moves,0);
}
int execute_command(Board **game_board, Command *command, LinkedList **game_moves, int is_redo) {
    Board* board = *game_board;
    LinkedList* moves= *game_moves;
    LinkedListCells* changed;
    int succeeded = 0;
    if(!is_redo){
        changed = alloc_linked_list_cells();
        if(command->type == SET || command->type == GENERATE || command->type == GUESS || command->type == AUTOFILL) {
            add_linked_list(moves, command, changed);
            advance_curr(moves);
        }
        else
            free(changed);
    }

    switch (command->type) {
        case SOLVE:
            board = load(get_filepath(command));
            *game_board = board;
            if(board!= NULL) {
                set_mode(1);
                succeeded = 1;
                print_board(board);
            }
            break;
        case EDIT:
            /*free_board(board);
            free_linked_list(moves);
            moves = alloc_linked_list_cells();*/
            board = load(get_filepath(command));
            *game_board = board;
            if(board!= NULL) {
                set_mode(2);
                succeeded = 1;
                print_board(board);
            }
            break;
        case MARK_ERRORS:
            mark_errors_command(get_parameters(command)[0]);
            break;
        case PRINT_BOARD:
            print_board(board);
            break;
        case SET:
            set_command(board, moves, get_parameters(command)[0], get_parameters(command)[1], get_parameters(command)[2]);
            succeeded = 1;
            print_board(board);
            break;
        case VALIDATE:
            break;
        case GUESS:
            break;
        case GENERATE:
            break;
        case UNDO:
            if (undo(board, moves)){
                succeeded = 1;
                print_board(board);
            }
            else
                command_error(8);
            break;
        case REDO:
            if(!redo(board, moves))
                command_error(9);
            else
                succeeded = 1;
            break;
        case SAVE:
            if(save(board, get_filepath(command)))
                succeeded =1;
            else
                succeeded =0;
            break;
        case HINT:
            break;
        case GUESS_HINT:
            break;
        case NUM_SOLUTIONS:
            printf("There are %d solutions for current board:\n",num_solutions_BT(board));
            print_board(board);
            break;
        case AUTOFILL:
            autofill_command(board, moves);
            succeeded =1;
            print_board(board);
            break;
        case RESET:
            reset_command(board, moves);
            succeeded =1;
            print_board(board);
            break;
        case EXIT:
            exit_command(board, moves);
            succeeded =1;
            break;
    }
    if(command->type != SET && command->type != GENERATE && command->type != GUESS && command->type != AUTOFILL) {
        free(command);
    }
    return succeeded;
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
    if (get_command(curr) == NULL) {
        return 0;
    } else {
        temp = curr;
        backward_curr(moves);
        change_cells_to(board, get_changed_cells_list(temp));
        return 1;
    }
}


int redo(Board *board, LinkedList *moves) {
    Command *c;
    if (is_curr_last(moves)) {
        return 0;
    }
    advance_curr(moves);
    c = get_command(get_curr(moves));
    /*c is one of: set/autofill/generate/guess */
    execute_command(&board,c,&moves,1);
    return 1;
}

void autofill_command(Board *board, LinkedList *moves) {
    autofill(board, moves);
}
int get_single_value(Board* board,int r, int c){
    int* sols = get_all_sol_cell(board, r, c);
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





