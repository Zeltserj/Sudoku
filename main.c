#include <stdio.h>
#include "board.h"
#include "game.h"
#include "parser.h"
#include "linkedList.h"
#include "commandValidator.h"
#include "fileaux.h"
#include "execute.h"
#include "command.h"

void print_input_parameters(Command *pCommand);

int main() {
    Board * board = load("C:\\Users\\lenovo\\CLionProjects\\SP\\test_faux.txt");
    LinkedList *moves = alloc_linkedList();
    LinkedListCells *LLC = alloc_linked_list_cells();
    Command *first_command = alloc_command("solve",NULL,NULL);
    add_linked_list(moves,first_command,LLC,0);
    while (1) {
        printf("please enter input:\n");
        char *str = calloc(256, sizeof(char));
        LinkedList *moves  = alloc_linkedList();
        Command *command = parse_input(fgets(str, 256, stdin));
        if (validate_command(command, board)) {
            /*printf("command type: %s\n", command_name(command));
            printf("input parameters: ");
            print_input_parameters(command);
            printf("\n");
            printf("file path: %s\n", get_filepath(command));
            printf("threshold: %.2f\n", get_threshold(command));
            printf("num parameters: %d\n", get_num_parameters(command));
      */
        execute_command(board,command,moves);
        }


    }


}

void print_input_parameters(Command *pCommand) {
    int i;
    if (get_parameters(pCommand) == NULL) {
        return;
    }
    for (i = 0; i < pCommand->num_parameters; i++) {
        printf("paramter %d: %d ", i, pCommand->int_parameters[i]);
    }

}