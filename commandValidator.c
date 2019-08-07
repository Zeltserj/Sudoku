/**

 Created by lenovo on ${DTE}
 **/

#include <stdio.h>
#include "commandValidator.h"
#include "board.h"


int validate_mark_errors(Command *command);

int validate_solve(Command *command);

int validate_edit(Command *command);

int validate_print_board(Command *command);

int validate_set(Command *command, Board *board);

int validate_validate(Command *command, Board *board);

int validate_guess(Command *command, Board *board);

int validate_generate(Command *command, Board *board);

int validate_undo_redo(Command *command);

int validate_save(Command *command, Board *board);

int validate_hint(Command *command, Board *board);

int validate_range(int *parameters, int size, int num_parameters);

int validate_num_solutions(Command *command, Board *board);

int validate_autofill(Command *command, Board *board);

int validate_reset(Command *command);

int validate_exit(Command *command);

int validate_command(Command *command, Board *board) {
    int out = 1;
    command_type type = get_type(command);
    switch (type) {
        case INVALID:
            out = 0;
            break;
        case SOLVE:
            out = validate_solve(command);
            break;
        case EDIT:
            out = validate_edit(command);
            break;
        case MARK_ERRORS:
            out = validate_mark_errors(command);
            break;
        case PRINT_BOARD:
            out = validate_print_board(command);
            break;
        case SET:
            out = validate_set(command, board);
            break;
        case VALIDATE:
            out = validate_validate(command, board);
            break;
        case GUESS:
            out = validate_guess(command, board);
            break;
        case GENERATE:
            out = validate_generate(command, board);
            break;
        case UNDO:
            out = validate_undo_redo(command);
            break;
        case REDO:
            out = validate_undo_redo(command);
            break;
        case SAVE:
            out = validate_save(command, board);
            break;
        case HINT:
            out = validate_hint(command, board);
            break;
        case GUESS_HINT:
            out = validate_hint(command, board);
            break;
        case NUM_SOLUTIONS:
            out = validate_num_solutions(command, board);
            break;
        case AUTOFILL:
            out = validate_autofill(command, board);
            break;
        case RESET:
            out = validate_reset(command);
            break;
        case EXIT:
            out = validate_exit(command);
            break;
        default:
            error("command validator", "validate_command", -1); /*shouldn't get here*/
            out = 0;
    }
    return out;
}

int validate_exit(Command *command) {
    int out = 1;
    if(get_num_parameters(command) != 0){
        command_error(7);
        out = 0;
    }
    return out;
}

int validate_reset(Command *command) {
    int out = 1;
    if(mode == 0){
        command_error(23);
        out = 0;
    }
    else if(get_num_parameters(command) != 0){
        command_error(7);
        out = 0;
    }
    return out;
}

/*TODO: Or: fix it later*/
int validate_autofill(Command *command, Board *board) {
    int out = 1;
    if(mode != 1){
        command_error(24);
        out = 0;
    }
    else if(get_num_parameters(command) != 0){
        command_error(7);
        out = 0;
    }
    return out;
}

int validate_num_solutions(Command *command, Board *board) {
    int out = 1;
    if(mode == 0){
        command_error(23);
        out = 0;
    }
    else if(get_num_parameters(command ) != 0){
        command_error(7);
        out = 0;
    }
    else if(is_erroneous(board)){
        command_error(26);
        out = 0;
    }
    return out;
}


int validate_hint(Command *command, Board *board) {
    int out = 1, r, c, *parameters = get_parameters(command);
    if (mode != 1) {
        command_error(24);
        out = 0;
    } else if (get_num_parameters(command) != 2) {
        command_error(7);
    } else {
        parameters[0]--;
        parameters[1]--;
        r = parameters[0];
        c = parameters[1];
        out = validate_range(parameters, get_size(board), get_num_parameters(command));
        if (out == 1) {
            if (is_erroneous(board)) {
                command_error(27);
                out = 0;
            } else if (is_fixed(board, r, c)) {
                command_error(26);
                out = 0;
            } else if (get(board, r, c) != 0) {
                command_error(28);
                out = 0;
            }
        }
    }
    return out;
}

int validate_save(Command *command, Board *board) {
    int out = 1, num_parameters = get_num_parameters(command);
    if (mode == 0) {
        command_error(23);
        out = 0;
    } else if (num_parameters != 1) {
        command_error(7);
        out = 0;
    } else if (mode == 2 && is_erroneous(board)) {
        command_error(27);
        out = 0;
    }
    return out;
}

int validate_undo_redo(Command *command) {
    int out = 1, num_parameters = get_num_parameters(command);
    if (mode == 0) {
        command_error(23);
        out = 0;
    } else if (num_parameters != 0) {
        command_error(7);
        out = 0;
    }
    return out;
}

int validate_generate(Command *command, Board *board) {
    int out = 1, num_parameters = get_num_parameters(command), *parameters = get_parameters(command);
    if (mode != 2) {
        command_error(25);
        out = 0;
    } else if (num_parameters != 2) {
        command_error(7);
        out = 0;
    } else if (parameters[0] > get_num_empty(board)) {
        command_error(20);
        out = 0;
    }
    return out;
}

int validate_guess(Command *command, Board *board) {
    int out = 1, num_parameters = get_num_parameters(command), is_board_invalid = is_erroneous(board);
    if (mode != 1) {
        command_error(24);
        out = 0;
    } else if (num_parameters != 1) {
        command_error(7);
        out = 0;
    } else if (get_threshold(command) > 1 || get_threshold(command) < 0) {
        command_error(20);
        out = 0;
    } else if (is_board_invalid) {
        command_error(27);
        out = 0;
    }
    return out;
}

int validate_validate(Command *command, Board *board) {
    int out = 1, num_parameters = get_num_parameters(command), is_board_invalid = is_erroneous(board);
    if (mode == 0) {
        command_error(23);
        out = 0;
    } else if (num_parameters != 0) {
        command_error(7);
        out = 0;
    } else if (is_board_invalid) {
        command_error(27);
        out = 0;
    }
    return out;
}

int validate_set(Command *command, Board *board) {
    int i, out = 1, num_parameters = get_num_parameters(command), *parameters = get_parameters(
            command), r, c, parameter_error = 20;

    if (mode == 0) {
        command_error(23);
        out = 0;
    } else if (num_parameters != 3) {
        command_error(7);
        out = 0;
    } else {
        out = validate_range(parameters, get_size(board), num_parameters-1);
        if(parameters[2]<0 || parameters[2]>get_size(board)){
            command_error(22);
            out = 0;
        }
        parameters[0]--;
        parameters[1]--;
        r = parameters[0];
        c = parameters[1];
        if (out == 1 && is_fixed(board, r, c)) {
            command_error(26);
            out = 0;
        }
    }
    return out;
}

int validate_range(int *parameters, int size, int num_parameters) {
    int i, out = 1;
    for (i = 0; i < num_parameters; i++) {
        if (parameters[i] < 1 || parameters[i] > size) {
            command_error(20 + i);
            out = 0;
            break;
        }
    }
    return out;
}

int validate_print_board(Command *command) {
    int out = 1, num_parameters = get_num_parameters(command);
    if (mode == 0) {
        command_error(23);
        out = 0;
    } else if (num_parameters != 0) {
        command_error(7);
        out = 0;
    }
    return out;
}

int validate_edit(Command *command) {
    int out = 1, num_parameters = get_num_parameters(command);
    if (num_parameters > 1) {
        command_error(7);
        out = 0;
    }
    return out;
}

int validate_solve(Command *command) {
    int out = 1, num_parameters = get_num_parameters(command);
    if (num_parameters != 1) {
        command_error(7);
        out = 0;
    }
    return out;
}

int validate_mark_errors(Command *command) {
    int out = 1, *parameters = get_parameters(command), num_parameters = get_num_parameters(command);
    if (mode != 1) {
        command_error(24);
        out = 0;
    } else if (num_parameters != 1) {
        command_error(7);
        out = 0;
    } else if (parameters[0] > 1 || parameters[0] < 0) {
        command_error(20);
        out = 0;
    }
    return out;
}


