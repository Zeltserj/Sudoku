
#ifndef SPROJECT_EXECUTE_H
#define SPROJECT_EXECUTE_H
#include "command.h"
#include "linkedList.h"
#include "board.h"

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

void set_command(Board *board, LinkedList *moves, int r, int c, int value);

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
void print_exit_command();

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* notice: if input mark_errors == 0, changed_cells can be NULL.
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return 1 if cell is valid for input value. 0 otherwise.
*/
int validate_cell(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors);

/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @return pointer to an array size board.size. each cell i is: 1 if i+1 is a legal solution for
* board[r][c], otherwise cell i is 0.
*/
int *get_all_sol_cell(Board *board, int r, int c);

#endif /*SPROJECT_EXECUTE_H*/
