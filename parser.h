/**

 Created by lenovo on ${DTE}
 **/


#ifndef SP_PARSER_H
#define SP_PARSER_H
#include "command.h"
/**
 *
 * @param input string from user
 * @return command type == invalid iff:
 * command does not use filepath
 * a non integer value was given to integer commands
 * input length > 256 chars
 *  non float was given to "guess" command
 *
 *
 * exits on memory failure. all else is given to command validator
 *
 */
Command* parse_input(char* input);
#endif
