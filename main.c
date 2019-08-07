#include <stdio.h>
#include "board.h"
#include "game.h"
#include "parser.h"
#include "linkedList.h"
#include "commandValidator.h"
#include "fileaux.h"
#include "execute.h"
#include "command.h"

int main() {
    Board * board = load("C:\\Users\\Or Nechemia\\CLionProjects\\SProject\\test_faux.txt");
    LinkedList *moves = alloc_linkedList();
    LinkedListCells *LLC = alloc_linked_list_cells();
    /*Command *first_command = alloc_command("solve",NULL,NULL);*/
    add_linked_list(moves,NULL,LLC,7);

    set_mode(0);
    /*set_command(board,moves,1,1,1);
    printf("is [0][0] error: %d",is_error(board,0,0));*/
    /*set_command(board,moves,0,0,1);
    autofill_command(board,moves);
    print_board(board);*/
    while (1) {
        printf("please enter input:\n");
        char *str = calloc(256, sizeof(char));
        /*LinkedList *moves  = alloc_linkedList();*/
        Command *command = parse_input(fgets(str, 256, stdin));
        if (validate_command(command, board)) {
            /*printf("command type: %s\n", command_name(command));
            printf("input parameters: ");
            print_input_parameters(command);
            printf("\n");
            printf("file path: %s\n", get_filepath(command));
            printf("num parameters: %d\n", get_num_parameters(command));
            printf("threshold: %.2f\n", get_threshold(command));
*/
            execute(&board, command, &moves);
        }
    }


}

