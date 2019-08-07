
#ifndef SP_GAME_H
#define SP_GAME_H

#include "board.h"

extern int mark_errors;
extern int mode; /*Init - 0, SOLVE - 1, EDIT - 2*/

void init_game();
/*int generate_game(Board *board, LinkedList *moves);*/

void print_exit_command();
void announce_winner();
void announce_erroneous();

#endif
