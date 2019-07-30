
#ifndef PROJECT_ERRORS_H
#define PROJECT_ERRORS_H
void error(char *module, char *function, int error_code);
void input_error(int errornum);
void command_error(int errornum);

#endif
