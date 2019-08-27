
#ifndef SP_GAME_H
#define SP_GAME_H
#define _INIT 0
#define _SOLVE 1
#define _EDIT 2

extern int mark_errors;
extern int mode; /*Init - 0, SOLVE - 1, EDIT - 2*/

void init_game();
/*int generate_game(Board *board, LinkedList *moves);*/

void print_exit_command();
void announce_winner();
void announce_erroneous();

#endif
