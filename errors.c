
#include <stdio.h>
#include "errors.h"


char *errors(int errornum) {
    char *out = NULL;
    switch (errornum) {
        case 1:
            out = "memory failure";
            break;
        case 2:
            out = "empty list";
            break;
        case 3:
            out = "next cell is null";
            break;
        case 4:
            out = "previous cell is null";
            break;
        case 5:
            out = "null name";
            break;
        case 6:
            out = "invalid command name";
            break;
        case 7:
            out = "incorrect number of parameters";
            break;
        case 8:
            out = "there are no moves to undo";
            break;
        case 9:
            out = "there are no moves to redo";
            break;
        case 10:
            out = "invalid parameter type";
            break;
        case 11:
            out = "input too long";
            break;
        case 12:
            out = "non-floating point value";
            break;
        case 13:
            out = "unsuccessful file opening";
            break;
        case 14:
            out = "unsuccessful file writing";
            break;
        case 15:
            out = "unsuccessful file reading";
            break;
        case 16:
            out = "file does not have enough numbers to fill a board";
            break;
        case 17:
            out = "file error: not all cell values are in range";
            break;
        case 18:
            out = "file error: not all cell values are numbers";
            break;
        case 19:
            out = "file has more arguments than needed";
            break;
        case 20:
            out = "first parameter out of range";
            break;
        case 21:
            out = "second parameter out of range";
            break;
        case 22:
            out = "third parameter out of range";
            break;
        case 23:
            out = "command unavailable in current mode, only in SOLVE or EDIT";
            break;
        case 24:
            out = "command unavailable in current mode, only in SOLVE";
            break;
        case 25:
            out = "command unavailable in current mode, only in EDIT";
            break;
        case 26:
            out = "cell is fixed";
            break;
        case 27:
            out = "board is erroneous";
            break;
        case 28:
            out = "cell is not empty";
            break;
        case 30:
            out = "first parameter has illegal value";
            break;
        case 31:
            out = "second parameter has illegal value";
            break;
        case 32:
            out = "third parameter has illegal value";
            break;
        default:
            out = "Unknown error";
    }

    return out;
}

void error(char *module, char *function, int error_code) {
    printf("Error: in module: %s, function %s : %s\n", module, function, errors(error_code));
}

void input_error(int errornum) {
    printf("Error: Invalid input: %s\n", errors(errornum));
}

void command_error(int errornum) {
    printf("Error: Invalid command: %s\n", errors(errornum));

}
