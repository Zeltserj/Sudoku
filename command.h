
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
typedef struct UserCommand{
    int num_parameters;
    enum CommandType type;
    int* int_parameters;
    char* string_parameters;
    float threshold;
    /*holds what type of error*/
}Command;


/**
 *
 * @param command != NULL
 * @return command.num_parameters
 */
int get_num_parameters(Command *command);
/**
 *
 * @param command != NULL
 * @return command.parameters
 */
int* get_parameters(Command* command);
/**
 *
 * @param command != NULL
 * @return command.filepath
 */
char* get_filepath(Command* command);
/**
 *
 * @param command != NULL
 * @return command.type
 */
command_type get_type(Command* command);
/**
 *
 * @param command != NULL
 * @return command.threshold
 */
float get_threshold(Command* command);
/**
 *
 * @param command != NULL
 * @param num_parameters
 * @return command.num_parameters == @param num_parameters
 */
void set_num_parameters(Command *command, int num_parameters);
/**
 *
 * @param command != NULL
 * @param parameters
 * @return command.parameters == @param parameters
 */
void set_parameter(Command *command, int *parameters, int num_parameters);
/**
 *
 * @param command != NULL
 * @param filepath
 * @return command.filepath == @param filepath
 */
void set_filepath(Command* command, char* filepath);
/**
 *
 * @param command != NULL
 * @param name
 * @return command.type == @param (type) name
 */
void set_type(Command *command, char *name);
/**
 *
 * @param command != NULL
 * @param threshold
 * @return command.threshold == @param threshold
 */
void set_threshold(Command *commnad, float threshold);
/**
 *
 * @param command != NULL
 * @return command is freed from heap
 */
int free_command(Command* command);

#endif
