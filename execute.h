
#ifndef SPROJECT_EXECUTE_H
#define SPROJECT_EXECUTE_H
#include "command.h"
#include "linkedList.h"
#include "board.h"

int execute(Board **game_board, Command *command, LinkedList **game_moves);
int execute_command(Board **game_board, Command *command, LinkedList **game_moves, int is_redo);
void set_mode(int new_mode);

/**
* undo the last move and change the cells of board respectively back to the values before that move
* @param board != NULL
* @param moves != NULL. A list of moves containd SET/ AUTOFILL/ GENERATE/ GUESS moves.
* move.current is the last move executed on board
* @return 1 if succeeded. otherwise 0 (means there are no moves to undo)
*/
int undo(Board *board, LinkedList *moves);


/**
* redo the next move after moves.current.
* @param board != NULL
* @param moves != NULL
* @return 1 if succeeded. otherwise 0 (no moves to redo)
*/
int redo(Board *board, LinkedList *moves);

void mark_errors_command(int mark);

/**
* fill in each board cell that has only one legal solution.
* @param board != NULL
*/
void autofill_command(Board *board, LinkedList *moves);
/**
* goes over all the possible solutions for the cell board[r][c].
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @return the single solution for the cell, otherwisr 0 - if there isn't a single solution
*/
int get_single_value(Board* board,int r, int c);



/**
* undo all moves and insert the pre-changed values to board.
* @param board != NULL
* @param moves != NULL
*/
void reset_command(Board *board, LinkedList *moves);

/**
* free all memory allocated for board and moves list
* @param board != NULL
* @param moves != NULL
*/
void exit_command(Board *board, LinkedList *moves);



#endif /*SPROJECT_EXECUTE_H*/
