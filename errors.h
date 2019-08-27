#include "gurobi_c.h"
#ifndef PROJECT_ERRORS_H
#define PROJECT_ERRORS_H
/**
 *
 * @param module is one of the program's .c files
 * @param function is an implemented function in @param module
 * @param error_code in range [1,33]
 * @return prints an error message informing the user where it happened and error type
 */
void error(char *module, char *function, int error_code);

/**
 *
 * @param errornum in range [1,33]
 * prints an error created by incorrect input (e.g. too many parameters or characters)
 */
void input_error(int errornum);

/**
 *
 * @param errornum in range [1,33]
 * prints an error created by command problem (e.g. erroneous board for autofill, no moves to undo)
 */
void command_error(int errornum);

/**
 *
 * @param error is a return value from a GRB function (e.g. GRBaddvars, GRBaddconstr)
 * prints the guriobi error to the user
 */
void gurobi_error(int _error, GRBenv *env);
#endif
