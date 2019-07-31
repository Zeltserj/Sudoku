
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"


char *errors(int errornum) {
    char *out = NULL;
    switch (errornum) {
        case 1:
            out = "memory failure\n";
            break;
        case 2:
            out = "empty list\n";
            break;
        case 3:
            out = "next cell is null\n";
            break;
        case 4:
            out = "previous cell is null\n";
            break;
        case 5:
            out = "null name\n";
            break;
        case 6:
            out = "invalid command name\n";
            break;
        case 7:
            out = "too many parameters\n";
            break;

        default:
            out = "Unknown error";
    }

    return out;
}

void error(char *module, char *function, int error_code) {
    printf("Error: in module: %s, function %s : %s", module, function, errors(error_code));
}

void input_error(int errornum) {
    printf("Error: Invalid input: %s\n", errors(errornum));
}

void command_error(int errornum) {
    printf("Error: Invalid command: %s\n", errors(errornum));

}
