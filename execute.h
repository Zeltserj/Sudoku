
#ifndef SPROJECT_EXECUTE_H
#define SPROJECT_EXECUTE_H
#include "command.h"
#include "linkedList.h"
#include "board.h"
/**
 * execute model takes in a valid command (validated by commandValidator) and is the "terminal" for execution.
 * it invokes functions in board modifier, fileaux, etc.
 */

/**
* executes the input command. For commands that change the board, updates game_moves so the current
* node contains the command and two lists of cell values that has been changed:
* one of the cell values before the command, and one for the values after the command.
* @param game_board != NULL. pointer to the address of the game board
* @param command != NULL. a legal command.
* @param game_moves != NULL. pointer to the address of the game board
* @return
*/
int execute_command(Board **game_board, Command *command, LinkedList **game_moves);

/**
 *
 * @param new_mode 0 ( init), 1 ( solver) or 2( (edit)
 */
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

/**
 *
 * @param mark == 1 || mark == 0
 * sets mark_errors in the game to mark
 */
void mark_errors_command(int mark);

/**
* fill in each board cell that has only one legal solution.
* @param board != NULL
*/
void autofill_command(Board *board, LinkedList *moves);




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
