
#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H
/**
 * Command structure, given as output from parser.
 * contains command type (enum), number of parameters and filepath or array of integers
 * if both are not NULL, the command is not valid.
 */
typedef enum CommandType{INVALID = 0, SOLVE, EDIT, MARK_ERRORS,
    PRINT_BOARD, SET, VALIDATE,
    GUESS, GENERATE, UNDO, REDO,
    SAVE, HINT, GUESS_HINT,
    NUM_SOLUTIONS, AUTOFILL, RESET,
    EXIT}command_type;
char* commands[] = {"invalid","solve","edit","mark_errors","print_board","set", "validate",
                    "guess","generate","undo","redo","save","hint","guess","num_solutions","autofill","reset","exit"};
typedef struct UserCommand{
    int num_parameters;
    enum CommandType type;
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

/**
 *
 * @param command
 * @return getters and setters for all attributes of Command structure
 */
int get_num_parameters(Command *command);
int* get_parameters(Command* command);
char* get_filepath(Command* command);
command_type get_type(Command* command);
void set_num_paramters(Command *command, int num_parameters);
void set_parameter(Command* command, int* parameters);
void set_filepath(Command* command, char* filepath);
void set_type(Command *command, char *name);
void free_command(Command* command);
/**
 *
 * @param command != NULL && command.type != NULL
 * @return  the name of the command
 *
 */
char * command_name(Command* command);
#endif
