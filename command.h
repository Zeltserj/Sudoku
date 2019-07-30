
#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H
typedef enum CommandType{INVALID = 0, SOLVE, EDIT, MARK_ERRORS,
    PRINT_BOARD, SET, VALIDATE,
    GUESS, GENERATE, UNDO, REDO,
    SAVE, HINT, GUESS_HINT,
    NUM_SOLUTIONS, AUTOFILL, RESET,
    EXIT}command_type;
typedef struct UserCommand{
    enum CommandType type;
     /*TODO: J: check if an error and none is necessary (seems so)*/
    int* int_parameters;
    char* string_parameters;
    /*holds what type of error*/
}Command;
/**
 *
 * @param name sets command type
 * @param parameters in case of commands other than edit and solve
 * @param filepath a string consisting of the file path.
 * @pre   parameters != NULL xor filepath != NULL
 * @return new command instance, ONLY COMMAND TYPE IS CHECKED
 */
Command* alloc_command(char* name, int* parameters, char* filepath);

int* get_parameters(Command* command);
char* get_filepath(Command* command);
command_type get_type(Command* command);
void set_parameter(Command* command, int* parameters);
void set_filepath(Command* command, char* filepath);
void set_type(Command *command, char *name);

void free_command(Command* command);
#endif //PROJECT_COMMAND_H
