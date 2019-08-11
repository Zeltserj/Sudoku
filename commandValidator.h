
#ifndef SP_COMMANDVALIDATOR_H
#define SP_COMMANDVALIDATOR_H

#include "command.h"
#include "board.h"

/**
 *
 * @param command
 * @return 1 iff command is valid 0 otherwise
 */
int validate_user_command(Command *command, Board *board);
#endif
