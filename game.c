
#include <stdio.h>
#include "game.h"
#include "parser.h"
#include "commandValidator.h"
#include "execute.h"

int mark_errors = 1;
int mode = _INIT;

void announce_winner();
void announce_erroneous();

void init_game() {
    Board *board = NULL;
    Command *command;
    LinkedList *moves = alloc_linkedList();
    char *str, *temp_str;
    int _exit = 0, exe_ret;
    add_linked_list(moves, NULL, NULL, NULL);
    command_type type;
    while (!_exit) {
        str = calloc(257, sizeof(char));
        command = (Command*)calloc(1, sizeof(Command));
        if (str == NULL || command == NULL) {
            error("game", "init_game", 1);
            exit(0);
        }
        printf("enter input:\n");
        temp_str = fgets(str, 257, stdin);
        if (temp_str == NULL) {
            if (feof(stdin)) {
                parse_input("exit", command);
                execute_command(&board, command, &moves);
                free_command(command);
                free(str);
                break;
            }
        }
        parse_input(str, command);
        type = get_type(command);
        if (validate_user_command(command, board)) {
            exe_ret = execute_command(&board, command, &moves);
            if (get_type(command) == EXIT) {
                free_command(command);
                _exit = 1;
            } else {
                if (type != SET && type != GENERATE && type != GUESS && type != AUTOFILL)
                    free_command(command);
                else if (get_type(command) == SET && mode == 1 && exe_ret && get_num_empty(board) == 0) {
                    if (!is_erroneous(board)) {
                        announce_winner();
                        mode = _INIT;
                    } else
                        announce_erroneous();
                }
            }
        }
        free(str);
    }
}

void announce_winner() {
    printf("You won! :)\n");
}

void announce_erroneous() {
    printf("The board is not correct (try to mark errors).\n");
}
