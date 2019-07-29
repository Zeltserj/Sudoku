#include <stdio.h>
#include "board.h"

int main() {
  Board* b = alloc_board(2, 3);
  set(b, 0, 0, 15);
  printf("%d\n", get(b,0,0));
  Board* ob = brdcpy(b);
  fix_cell(ob, 1, 1);
  printf("b: %d\nob: %d\n", is_fixed(b, 1,1), is_fixed(ob, 1,1));
  free_board(b);
  printf("b is freeeee");
}
