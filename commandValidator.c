/**

 Created by lenovo on ${DTE}
 **/


#include "commandValidator.h"

int validate_command(Command *command) {
    int out = 1;
    int num_parameters = get_num_parameters(command);
    command_type type = get_type(command);
    int *parameters = get_parameters(command);
    char *filepath = get_filepath(command);
    switch (type) {
        case INVALID:
            out = 0;
            break;
        case SOLVE:
            if (num_parameters != 1) {
                command_error(7);
                out = 0;
            }
            break;
        case EDIT:
            if (num_parameters > 1) {
                command_error(7);
                out = 0;
            }
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
    return out;
}

