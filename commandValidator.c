/**

 Created by lenovo on ${DTE}
 **/


#include "commandValidator.h"

int validate_command(Command *command) {
    int out = 1;
    switch (command->type){
        case INVALID:
            input_error(6);
            out = 0;
            break;
        case SOLVE:
            break;
        case EDIT:
            break;
        case MARK_ERRORS:
            break;
        case PRINT_BOARD:
            break;
        case SET:
            break;
        case VALIDATE:
            break;
        case GUESS:
            break;
        case GENERATE:
            break;
        case UNDO:
            break;
        case REDO:
            break;
        case SAVE:
            break;
        case HINT:
            break;
        case GUESS_HINT:
            break;
        case NUM_SOLUTIONS:
            break;
        case AUTOFILL:
            break;
        case RESET:
            break;
        case EXIT:
            break;
    }
    return 0;
}

