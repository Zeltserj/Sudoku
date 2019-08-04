#ifndef SPROJECT_SOLVER_H
#define SPROJECT_SOLVER_H

#include "board.h"

/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @return pointer to an array size board.size. each cell i is: 1 if i+1 is a legal solution for
* board[r][c], otherwise cell i is 0.
*/
int *get_all_sol_cell(Board *board, int r, int c);

#endif /*SPROJECT_SOLVER_H*/
