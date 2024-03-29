
#include <stdio.h>
#include "execute.h"
#include "boardModifier.h"
#include "fileaux.h"

void print_probability_array(double* prob, int len);
int edit_solve_command(Board **game_board, LinkedList *moves, Command *command, int edit_or_solve);
int undo_redo_command(Board *board, LinkedList *moves, int undo_or_redo);
int valid_board(Board *board);
void update_conflicts(Board *to);

/*the first node in moves has command==NULL as the basic list. Therefore moves is not empty*/
int execute_command(Board **game_board, Command *command, LinkedList **game_moves) {
    Board *board = *game_board;
    LinkedList *moves = *game_moves;
    LinkedListCells *old_values, *new_values;
    int succeeded = 0, *parameters = get_parameters(command), hint;
    double *sol_prob;
    command_type type = get_type(command);
    if (type == SET || type == GENERATE || type == GUESS || type == AUTOFILL) {
        old_values = alloc_linked_list_cells();
        new_values = alloc_linked_list_cells();
        add_linked_list(moves, command, old_values, new_values);
        advance_curr(moves);
    }
    switch (type) {
        case SOLVE:
            succeeded = edit_solve_command(game_board, moves, command, 1);
            break;
        case EDIT:
            succeeded = edit_solve_command(game_board, moves, command, 0);
            break;
        case MARK_ERRORS:
            mark_errors_command(parameters[0]);
            break;
        case PRINT_BOARD:
            print_board(board);
            break;
        case SET:
            set_command(board, moves, parameters[0], parameters[1], parameters[2], 1);
            succeeded = 1;
            print_board(board);
            break;
        case VALIDATE:
            if (validate_command(board)) {
                succeeded = 1;
                printf("board is solvable\n");
            } else
                printf("board is not solvable\n");
            break;
        case GUESS:
            succeeded = guess_command(board, moves, get_threshold(command));
            if (succeeded)
                print_board(board);
            else
                command_error(33);
            break;
        case GENERATE:
            if (generate_command(board, moves, parameters[0], parameters[1])) {
                succeeded = 1;
                print_board(board);
            } else
                printf("error in the puzzle generator\n");
            break;
        case UNDO:
            succeeded = undo_redo_command(board,moves,0);
            break;
        case REDO:
            succeeded = undo_redo_command(board,moves,1);
            break;
        case SAVE:
            succeeded = save(board, get_filepath(command));
            break;
        case HINT:
            hint = hint_command(board, parameters[0], parameters[1]);
            if (hint>0) {
                succeeded = 1;
                printf("hint: try to set %d in cell (%d,%d)\n", hint, parameters[1] + 1, parameters[0] + 1);
            }
            else
                command_error(33);
            break;
        case GUESS_HINT:
            sol_prob = guess_hint_command(board, parameters[0], parameters[1]);
            if (sol_prob != NULL) {
                print_probability_array(sol_prob, get_size(board));
                succeeded = 1;
            } else
                command_error(33);
            free(sol_prob);
            break;
        case NUM_SOLUTIONS:
            printf("There are %d solutions for current board\n", num_solutions_BT(board));
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
    printf("Exiting...\n");
}

/**
 *
 * @param prob != NULL
 * @param len == board.size for current game board
 *
 * prints the probability array generated by the LP
 */

void print_probability_array(double* prob, int len) {
    int i;
    printf("the possible solutions for the cell are:\n");
    for (i = 0; i < len; i++) {
        if (prob[i] > 0)
            printf("%d : %f\n", i + 1, prob[i]);
    }
}

/**
* load the board in file get_filepath(command) to &game_board. for edit without path, sets empty 9X9 board.
* if succeeded, sets the correct mode, empty list of moves and prints the board loaded.
* @param game_board != NULL.
* @param moves != NULL. should be allocated already
* @param command != NULL. type_command(command) is EDIT or SOLVE
* @param edit_or_solve. 0 for edit, 1 for solve.
* @return 1 if succeeded, 0 otherwise.
*/
int edit_solve_command(Board **game_board, LinkedList *moves, Command *command, int edit_or_solve) {
    Board* board, *temp;/* *new_board=NULL;*/
    char* file_path = get_filepath(command);
    if(*game_board != NULL){
        temp = *game_board;
       free_board(temp);
       clear_linked_list(moves);
    }
    if(get_num_parameters(command) == 0)
        board = generate_basic_board();
    else{
        board = load(file_path, edit_or_solve);
        if(board != NULL){
            if(!valid_board(board) ){
                command_error(34);
                free_board(board);
                return 0;
            }
            update_conflicts(board);
        }
    }
    *game_board =board;
    if (board != NULL) {
        if(edit_or_solve)
            set_mode(1); /*EDIT*/
        else
            set_mode(2); /*SOLVE*/
        clear_linked_list(moves);
        print_board(board);

        return 1;
    }
    return 0;
}

/**
 *
 * @param board != NULL
 *
 * at termination, board[i][j] == 0 iff cell is not fixed
 */
void delete_not_fixed(Board *board){
    int r,c, size=get_size(board);
    for(r=0; r<size; r++){
        for(c=0;c<size; c++){
            if(!is_fixed(board,r,c))
                set_value(board,r,c,0);
        }
    }
}
/**
 *
 * @param board != NULL
 * @return 1 iff the board is legal and contains 0 conflicts of values among fixed cells only
 */
int valid_board(Board *board){
    int r, c, size=get_size(board), value;
    Board* brd_cpy = brdcpy(board);
    delete_not_fixed(brd_cpy);
    for(r=0; r<size; r++){
        for(c=0;c<size; c++){
            value = get(brd_cpy,r,c);
            if(!validate_cell(brd_cpy,NULL,NULL,r,c,value,0)) {
                free_board(brd_cpy);
                return 0;
            }
        }
    }
    free_board(brd_cpy);
    return 1;
}

/**
 *
 * @param to != NULL
 * for every i,j in range [0, get_size(to)) to[i][j].errors == number of conflicting cells (exclusive) at termination
 */
void update_conflicts(Board *to) {
    int r,c, size=get_size(to), value;
    for(r=0; r<size; r++){
        for(c=0;c<size; c++){
            value= get(to,r,c);
            validate_cell(to,NULL,NULL,r,c,value,2);
        }
    }
}

/**
* undo the current move of moves / redo the next move of moves (as required by undo_or_redo).
* if succeeded, prints board. otherwise, prints appropriate error.
* @param board != NULL
* @param moves != NULL. the current of moves is the last move done.
* @param undo_or_redo - 1 for undo, 0 for redo
* @return 1 if succeeded, otherwise 0
*/
int undo_redo_command(Board *board, LinkedList *moves, int undo_or_redo) {
    int succeeded, error_num = 8;
    if (undo_or_redo)
        succeeded = redo(board, moves);
    else
        succeeded = undo(board, moves);

    if (succeeded) {
        print_board(board);
        return 1;
    }

    error_num += undo_or_redo;
    command_error(error_num);
    return 0;
}
