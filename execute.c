
#include <stdio.h>
#include "execute.h"
#include "boardModifier.h"
#include "fileaux.h"

void print_probability_array(double* prob, int len);
int load_command(Board **game_board, LinkedList *moves, Command *command, int edit_or_solve);

/*the first node in moves has command==NULL therefore moves is not empty*/
/*int execute(Board **game_board, Command *command, LinkedList **game_moves){
    return execute_command(game_board, command, game_moves);
}*/
int execute_command(Board **game_board, Command *command, LinkedList **game_moves) {
    Board* board = *game_board;
    LinkedList* moves= *game_moves;
    LinkedListCells *old_values, *new_values;
    int succeeded = 0, *parameters = get_parameters(command), hint;
    double* sol_prob;
    /*if(!is_redo){*/
        if(command->type == SET || command->type == GENERATE || command->type == GUESS || command->type == AUTOFILL) {
            old_values = alloc_linked_list_cells();
            new_values = alloc_linked_list_cells();
            add_linked_list(moves, command, old_values, new_values);
            advance_curr(moves);
        }
    /*}*/
    switch (command->type) {
        case SOLVE:
            succeeded = load_command(game_board, moves, command, 1);
            break;
        case EDIT:
            /*TODO: Or: need to add no-path function*/
            succeeded = load_command(game_board, moves, command, 0);
            break;
        case MARK_ERRORS:
            mark_errors_command(parameters[0]);
            break;
        case PRINT_BOARD:
            print_board(board);
            break;
        case SET:
            /*TODO: Or: only in solve mode fixed cells cannot be updated*/
            set_command(board, moves, parameters[0], parameters[1],parameters[2]);
            succeeded = 1;
            print_board(board);
            break;
        case VALIDATE:
            if (validate_command(board)) {
                succeeded = 1;
                printf("board is solvable.\n");
            } else
                printf("board is not solvable.\n");
            break;
        case GUESS:
            succeeded = guess_command(board,moves,get_threshold(command));
            if(succeeded)
                print_board(board);
            break;
        case GENERATE:
            if(generate_command(board, moves, parameters[0], parameters[1])) {
                succeeded =1;
                print_board(board);
            }
            else
                printf("error in the puzzle generator.\n");
            break;
        case UNDO:
            if (undo(board, moves)) {
                succeeded = 1;
                print_board(board);
            } else
                command_error(8);
            break;
        case REDO:
            if (!redo(board, moves))
                command_error(9);
            else{
                succeeded = 1;
                print_board(board);
            }
            break;
        case SAVE:
            succeeded = save(board, get_filepath(command));
            break;
        case HINT:
            hint = hint_command(board,parameters[0],parameters[1]);
            if(hint){
                printf("hint: try to set %d in cell (%d,%d).\n",hint,parameters[0]+1,parameters[1]+1);
                succeeded = 1;
            }
            else
                command_error(33);
            break;
        case GUESS_HINT:
            sol_prob = guess_hint_command(board,parameters[0],parameters[1]);
            if(sol_prob!= NULL){
                print_probability_array(sol_prob,get_size(board));
                succeeded=1;
            }
            else
                command_error(33);
            free(sol_prob);
            break;
        case NUM_SOLUTIONS:
            printf("There are %d solutions for current board.\n", num_solutions_BT(board));
            break;
        case AUTOFILL:
            autofill_command(board, moves);
            succeeded = 1;
            print_board(board);
            break;
        case RESET:
            reset_command(board, moves);
            succeeded = 1;
            print_board(board);
            break;
        case EXIT:
            exit_command(board, moves);
            succeeded = 1;
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
    Node *curr = get_curr(moves);
    if (get_command(curr) == NULL) {
        return 0;
    }
    temp = curr;
    backward_curr(moves);
    change_cells_to(board, get_old_values_cells_list(temp));
    return 1;
}


int redo(Board *board, LinkedList *moves) {
    Node *curr;
    if (is_curr_last(moves)) {
        return 0;
    }
    advance_curr(moves);
    curr = get_curr(moves);
    change_cells_to(board, get_new_values_cells_list(curr));
    return 1;

}

void autofill_command(Board *board, LinkedList *moves) {
    autofill(board, moves);
}


void reset_command(Board *board, LinkedList *moves) {
    while (undo(board,moves)){}
}

void exit_command(Board *board, LinkedList *moves) {
    free_board(board);
    free_linked_list(moves);
}

void print_probability_array(double* prob, int len) {
    int i;
    printf("the possible solutions for the cell are:\n");
    for (i = 0; i < len; i++) {
        if (prob[i] > 0)
            printf("%d : %f\n", i + 1, prob[i]);
    }
}

int load_command(Board **game_board, LinkedList *moves, Command *command, int edit_or_solve) {
    Board* board = load(get_filepath(command), edit_or_solve);
    *game_board = board;
    if (board != NULL) {
        if(edit_or_solve)
            set_mode(1);
        else
            set_mode(2);
        clear_linked_list(moves);
        print_board(board);
        return 1;
    }
    return 0;
}



