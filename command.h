
#ifndef PROJECT_COMMAND_H
#define PROJECT_COMMAND_H
typedef struct UserCommand{
    enum type{solve, edit, mark_errors,
            print_board, set, validate,
            guess, generate, undo, redo,
            save, hint, guess_hint,
            num_solutions, autofill, reset,
            exit};
    int* int_parameters;
    char** string_parameters;
    int is_valid;
    int error_code; /*holds what type of error
}Command;
#endif //PROJECT_COMMAND_H
