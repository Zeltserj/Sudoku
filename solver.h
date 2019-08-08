#ifndef SPROJECT_SOLVER_H
#define SPROJECT_SOLVER_H

#include "board.h"
#include "linkedList.h"
/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @return pointer to an array size board.size. each cell i is: 1 if i+1 is a legal solution for
* board[r][c], otherwise cell i is 0.
*/
int *get_all_sol_cell(Board *board, int r, int c);
int ILP_solve(Board *board, int *super_array);
int generate_solution(Board* board);

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* notice: if input mark_errors == 0, changed_cells can be NULL.
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return 1 if cell is valid for input value. 0 otherwise.
*/
int validate_cell(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int inc_or_dec);

#endif /*SPROJECT_SOLVER_H*/
