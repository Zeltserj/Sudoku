/**

 Created by lenovo on ${DTE}
 **/


#ifndef SP_COMMANDVALIDATOR_H
#define SP_COMMANDVALIDATOR_H

#include "errors.h"
#include "command.h"
#include "board.h"

/**
 *
 * @param command
 * @return 1 iff command is valid 0 otherwise
 */
int validate_command(Command *command, Board *board);
#endif
