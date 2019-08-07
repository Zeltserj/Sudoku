
#include <stdio.h>
#include "game.h"
#include "parser.h"
#include "commandValidator.h"
#include "board.h"
#include "execute.h"

int mark_errors = 1;
int mode = 1;

void init_game() {
    Board* board;
    Command *command;
    LinkedList* moves = alloc_linkedList();
    char *str = calloc(256, sizeof(char));
    int exit=0, won=0, exe_ret;
    mode = 0;
    add_linked_list(moves,NULL,NULL,0);
    while(!exit && !won){
        printf("enter input:\n");
        command = parse_input(fgets(str, 256, stdin));
        if (validate_command(command, board)){
            if(get_type(command)== EXIT){
                print_exit_command();
                exit = 1;
            }
            else{
                exe_ret = execute(&board,command,&moves);
                if(get_type(command) == SET && exe_ret && get_num_empty(board) == 0){
                    if(!is_erroneous(board)) {
                        announce_winner();
                        won = 1;
                    }
                    else {
                        announce_erroneous();
                    }
                }
            }
        }
    }
    if(won)
        mode=0;

}

void print_exit_command() {
    printf("Exiting...\n");
}

void announce_winner() {
    printf("You won! :)\n");
}

void announce_erroneous() {
    printf("The board is not correct (try to mark errors).\n");
}
