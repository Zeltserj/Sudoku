
#ifndef SP_GAME_H
#define SP_GAME_H
#define _INIT 0
#define _SOLVE 1
#define _EDIT 2

/**
 * the game module is the gateway to the game, it hold global variables of mode and mark_errors and initializes the game
 * calling parser -> commandValidator -> execute -> *relevant module* -> parser while no hard errors occur and the user doesn't exit
 * the ONLY module to get user input
 */
extern int mark_errors;
extern int mode; /*Init - 0, SOLVE - 1, EDIT - 2*/

void init_game();


#endif
