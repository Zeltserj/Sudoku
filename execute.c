
#include "execute.h"
#include "game.h"

int execute_command(Command *command) {
    switch (command->type){
        case SOLVE:break;
        case EDIT:break;
        case MARK_ERRORS:break;
        case PRINT_BOARD:break;
        case SET:break;
        case VALIDATE:break;
        case GUESS:break;
        case GENERATE:break;
        case UNDO:break;
        case REDO:break;
        case SAVE:break;
        case HINT:break;
        case GUESS_HINT:break;
        case NUM_SOLUTIONS:break;
        case AUTOFILL:break;
        case RESET:break;
        case EXIT:break;
    }
}

void set_mode(int newmode) {
     mode = newmode;
}
