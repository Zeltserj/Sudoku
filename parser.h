/**

 Created by lenovo on ${DTE}
 **/


#ifndef SP_PARSER_H
#define SP_PARSER_H
#include "command.h"
/**
 *
 * @param input string from user
 * @return command type == invalid iff command does not use filepath
 * and a non integer was given or input length > 256 chars. exits on memory failure. all else is given to command validator
 *
 */
Command* parse_input(char* input);
#endif
