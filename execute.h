
#ifndef SPROJECT_EXECUTE_H
#define SPROJECT_EXECUTE_H
#include "command.h"
#include "linkedList.h"

int execute_command(Board *board, Command *command, LinkedList *moves);
void set_mode(int new_mode);

/**
* undo the last move and change the cells of board respectively back to the values before that move
* @param board != NULL
* @param moves != NULL. A list of moves containd SET/ AUTOFILL/ GENERATE/ GUESS moves.
* move.current is the last move executed on board
* @return 1 if succeeded. otherwise 0 (means there are no moves to undo)
*/
int undo(Board *board, LinkedList *moves);

/** update board cells to the values of the cells in old_values (error/value/fixed)
* @param board != NULL
* @param old_values != NULL. a list of NodeCell
*/
void change_cells_to(Board *board, LinkedListCells *old_values);

/**
* redo the next move after moves.current.
* @param board != NULL
* @param moves != NULL
* @return 1 if succeeded. otherwise 0 (no moves to redo)
*/
int redo(Board *board, LinkedList *moves);

void mark_errors_command(int mark);


int autofill_command(Board* board);
/**
* goes over all the possible solutions for the cell board[r][c].
* @param board != NULL
* @param r < board.size
* @param c < board.size
* @return the single solution for the cell, otherwisr 0 - if there isn't a single solution
*/
int get_single_value(Board* board,int r, int c);

int set_command(Board* board,int r, int c, int value);

#endif /*SPROJECT_EXECUTE_H*/
