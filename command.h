
#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H
typedef struct UserCommand{
    enum CommandType{invalid = 0, solve, edit, mark_errors,
            print_board, _set, validate,
            guess, generate, undo, redo,
            save, hint, guess_hint,
            num_solutions, autofill, reset,
            exit_}type; /*TODO: J: check if an error and none is necessary (seems so)*/
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

#endif //PROJECT_COMMAND_H
