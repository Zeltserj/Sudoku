#include <stdio.h>
#include "board.h"
#include "game.h"

int main() {
   Board *board = alloc_board(5, 3);
   set(board, 0, 2, 4);
   set(board, 1, 1, 3);
   fix_cell(board,1,1);
   print_board(board);
    return 0;
}
