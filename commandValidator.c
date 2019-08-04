/**

 Created by lenovo on ${DTE}
 **/


#include "commandValidator.h"
#include "board.h"


int validate_command(Command *command, Board *board) {
    int i, out = 1, parameter_error = 20, size = get_size(board),r,c,value;  /*parameter location error codes start at 20*/
    int num_parameters = get_num_parameters(command), is_board_invalid = is_erroneous(board);
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
            if (num_parameters != 1) {
                command_error(7);
                out = 0;
            } else if (parameters[0] > 1 || parameters[0] < 0) {
                command_error(20);
                out = 0;
            } else if (mode != 1) {
                command_error(24);
                out = 0;
            }
            break;
        case PRINT_BOARD:
            if(mode == 0){
                command_error(23);
                out = 0;
            }
            else if (num_parameters != 0) {
                command_error(7);
                out = 0;
            } else if (mode == 0) {
                command_error(23);
                out = 0;
            }
            break;
        case SET:
            if(mode == 0){
                command_error(23);
                out = 0;
            }
            else if (num_parameters != 3) {
                command_error(7);
                out = 0;
            } else {
                parameters[0]--;
                parameters[1]--;
                r = parameters[0];
                c = parameters[1];
                value = parameters[2];
                for (i = 0; i < num_parameters; i++) {
                    if (parameters[i] > size || parameters[i] < 0) {
                        out = 0;
                        command_error(parameter_error + i);
                        break;
                    }

                }
                if(is_fixed(board,r,c)){
                    out = 0;
                    command_error(26);
                }
            }


            break;
        case VALIDATE:
            if(mode == 0){
                out = 0;
                command_error(23);
            }
            else if(is_board_invalid){
                out = 0;
                command_error(27);
            }
            break;
        case GUESS:
            if(mode != 1){
                command_error(24);
                out = 0;
            }
            else if(num_parameters != 1){
                command_error(7);
                out = 0;
            }
            else if(get_threshold(command) > 1 || get_threshold(command) < 0){
                command_error(23);
                out = 0;
            }
            else if(is_board_invalid){
                command_error(27);
                out = 0;
            }
            break;
        case GENERATE:
            if(mode != 2){
                command_error(25);
                out = 0;
            }
            else if(num_parameters != 2){
                command_error(7);
                out = 0;
            }
            else if(parameters[0] > get)
            break;
        case UNDO:
            if(mode == 0){
                command_error(23);
                out = 0;
            }
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
        default:
            error("command validator","validate_command", -1); /*shouldn't get here*/
            out = 0;
    }
    return out;
}

