
#ifndef SPROJECT_EXECUTE_H
#define SPROJECT_EXECUTE_H
#include "command.h"
#include "linkedList.h"

int execute_command(Command* command);
void set_mode(int new_mode);

/**
* undo the last move and change the cells of board respectively back to the values before that move
* @param board != NULL
* @param moves != NULL. A list of moves containd SET/ AUTOFILL/ GENERATE/ GUESS moves.
* move.current is the last move executed on board
*/
void undo(Board *board, LinkedList *moves);

void redo(Board *board, LinkedList *moves);
void mark_errors_command(int mark);
void print_board_command(Board* board);

#endif /*SPROJECT_EXECUTE_H*/