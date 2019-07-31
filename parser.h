/**

 Created by lenovo on ${DTE}
 **/


#ifndef SP_PARSER_H
#define SP_PARSER_H
#include "Command.h"
Command* parse_input(char* input);



int *parse_input_parameters(char *string, Command *command); /*TODO: delete these from header, for test purposes only*/

char *parse_filepath(char *string);
#endif
