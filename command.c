/**

 Created by lenovo on ${DTE}
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "command.h"



Command *alloc_command(char *name, int *parameters, char *filepath) {
    Command *command = (Command *) calloc(1, sizeof(Command));
    if (command == NULL) {
        error("command", "alloc_command", 1);
        return NULL;
    }
    if (name == NULL) {
        command->type = INVALID;
        error("command", "alloc_command", 5);
        return command;
    }
    set_type(command, name);
    command->int_parameters = parameters;
    command->string_parameters = filepath;
    return command;
}

int *get_parameters(Command *command) {
    return command->int_parameters;
}

/**
 *
 * @param command != NULL
 * @param name != null
 */
void set_type(Command *command, char *name) {
    if (strcmp(name, "SOLVE") == 0) {
        command->type = SOLVE;
    }
    if (strcmp(name, "EDIT") == 0) {
        command->type = EDIT;
    }
    if (strcmp(name, "MARK_ERRORS") == 0) {
        command->type = MARK_ERRORS;
    }
    if (strcmp(name, "PRINT_BOARD") == 0) {
        command->type = PRINT_BOARD;
    }
    if (strcmp(name, "set") == 0) {
        command->type = SET;
    }
    if (strcmp(name, "VALIDATE") == 0) {
        command->type = VALIDATE;
    }
    if (strcmp(name, "GUESS") == 0) {
        command->type = GUESS;
    }
    if (strcmp(name, "GENERATE") == 0) {
        command->type = GENERATE;
    }
    if (strcmp(name, "UNDO") == 0) {
        command->type = UNDO;
    }
    if (strcmp(name, "REDO") == 0) {
        command->type = REDO;
    }
    if (strcmp(name, "SAVE") == 0) {
        command->type = SAVE;
    }
    if (strcmp(name, "HINT") == 0) {
        command->type = HINT;
    }
    if (strcmp(name, "GUESS_HINT") == 0) {
        command->type = GUESS_HINT;
    }
    if (strcmp(name, "NUM_SOLUTIONS") == 0) {
        command->type = NUM_SOLUTIONS;
    }
    if (strcmp(name, "AUTOFILL") == 0) {
        command->type = AUTOFILL;
    }
    if (strcmp(name, "RESET") == 0) {
        command->type = RESET;
    }
    if (strcmp(name, "exit") == 0) {
        command->type = EXIT;
    } else {
        command->type = INVALID;
    }


}

command_type get_type(Command *command) {
    return command->type;
}

char *get_filepath(Command *command) {
    return command->string_parameters;
}

void set_filepath(Command *command, char *filepath) {
    command->string_parameters = filepath;
}

void set_parameter(Command *command, int *parameters) {
    command->int_parameters = parameters;

}

void free_command(Command *command) {
    free(command->int_parameters);
    free(command->string_parameters);
    free(command);
}


