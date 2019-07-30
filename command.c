/**

 Created by lenovo on ${DTE}
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "command.h"

void set_type(Command *command, char *name);

Command *alloc_command(char *name, int *parameters, char *filepath) {
    Command *command = (Command*)calloc(1, sizeof(Command));
    if(command == NULL){
        error("command", "alloc_command", 1);
        return NULL;
    }
    if(name == NULL){
        command->type = invalid;
        error("command","alloc_command",5);
        return command;
    }
    set_type(command, name);
    command->int_parameters = parameters;
    command->string_parameters = filepath;
    return command;
}
/**
 *
 * @param command != NULL
 * @param name != null
 */
void set_type(Command *command, char *name) {
    if(strcmp(name, "solve") == 0){
        command->type = solve;
    }
    if(strcmp(name, "edit") == 0){
        command->type = edit;
    }
   if(strcmp(name, "mark_errors") == 0){
        command->type = mark_errors;
    }
    if(strcmp(name, "print_board") == 0){
        command->type = print_board;
    }
    if(strcmp(name, "set") == 0){
        command->type = set;
    }
    if(strcmp(name, "validate") == 0){
        command->type = validate;
    }
    if(strcmp(name, "guess") == 0){
        command->type = guess;
    }
    if(strcmp(name, "generate") == 0){
        command->type = generate;
    }
    if(strcmp(name, "undo") == 0){
        command->type = undo;
    }
    if(strcmp(name, "redo") == 0){
        command->type = redo;
    }
    if(strcmp(name, "save") == 0){
        command->type = save;
    }
    if(strcmp(name, "hint") == 0){
        command->type = hint;
    }
    if(strcmp(name, "guess_hint") == 0){
        command->type = guess_hint;
    }
    if(strcmp(name, "num_solutions") == 0){
        command->type = num_solutions;
    }
    if(strcmp(name, "autofill") == 0){
        command->type = autofill;
    }
    if(strcmp(name, "reset") == 0){
        command->type = reset;
    }
    if(strcmp(name, "exit") == 0) {
        command->type = exit_;
    }
    else{
        command->type = invalid;
    }
}
