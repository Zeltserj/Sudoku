
#ifndef SP_COMMANDVALIDATOR_H
#define SP_COMMANDVALIDATOR_H

#include "command.h"
#include "board.h"

/**
 * commandValidator module accepts a command struct and validates it according to the project instructions.
 * the command accepted was parsed by parser, so input errors (e.g. more than 256 chars) do not appear here.
 *
 */


/**
 *
 * @param command
 * @return 1 iff command is valid 0 otherwise
 */
int validate_user_command(Command *command, Board *board);
#endif
