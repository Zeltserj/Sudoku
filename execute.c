
#include <stdio.h>
#include "execute.h"
#include "game.h"
#include "solver.h"
#include "cell.h"
#include "fileaux.h"
#include "command.h"


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
            add_linked_list(moves, command, changed, mode);
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
            print_exit_command();
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
    if ( get_command(curr) == NULL) {
        return 0;
    } else {
        temp = curr;
        backward_curr(moves);
        change_cells_to(board, get_changed_cells_list(temp));
        printf("linked list in undo:\n");
        print_linked_list(moves);
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
    Board* b_cpy = brdcpy(board);
    int i,j,v;
    for(i=0; i<get_size(board);i++){
        for(j=0;j<get_size(board);j++){
            if(get(b_cpy,i,j) == 0) {
                v = get_single_value(b_cpy, i, j);
                if (v != 0) {
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
}



