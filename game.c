
#include <stdio.h>
#include "game.h"
#include "parser.h"
#include "commandValidator.h"
#include "execute.h"

int mark_errors = 1;
int mode = _INIT;

/*TODO: make sure program exits in EOF */

void init_game() {
    Board *board = NULL;
    Command *command;
    LinkedList *moves = alloc_linkedList();
    char *str = calloc(257, sizeof(char));
    int exit = 0, exe_ret, freed;
    add_linked_list(moves, NULL, NULL, NULL);
    command_type type;
    while (!exit) {

        printf("enter input:\n");
        str = fgets(str, 257, stdin);
        if (str == NULL) {
            if (feof(stdin)) {
                print_exit_command();
                command = parse_input("exit");
                execute_command(&board, command, &moves);
                free_command(command);
                break;
            }
        }
        command = parse_input(str);
        type = get_type(command);
        if (validate_user_command(command, board)) {
            if (get_type(command) == EXIT) {
                execute_command(&board, command, &moves);
                free_command(command);
                print_exit_command();
                exit = 1;
            } else {
                exe_ret = execute_command(&board, command, &moves);

                if (type != SET && type != GENERATE && type != GUESS && type != AUTOFILL)
                    free_command(command);

                else if (get_type(command) == SET && mode == 1 && exe_ret && get_num_empty(board) == 0) {
                    if (!is_erroneous(board)) {
                        announce_winner();
                        mode = _INIT;
                    } else {
                        announce_erroneous();
                    }
                }
            }
        }
    }
}



/*TODO: Or: move to execute*/
void print_exit_command() {
    printf("Exiting...\n");
}

void announce_winner() {
    printf("You won! :)\n");
}

void announce_erroneous() {
    printf("The board is not correct (try to mark errors).\n");
}
