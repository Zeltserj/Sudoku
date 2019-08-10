/**
This module communicates with the gurobi optimizer in order to solve a n*m*m*n Sudoku board using ILP or at least making strong guess using LP.
 In both cases, a super array of length (mn)^3 is used in order to index variables.

 **/


#ifndef SP_GUROBISOLVER_H
#define SP_GUROBISOLVER_H

#include "board.h"

#define ILP 1
#define LP 0
/**
 *
 * @param board is not erroneous, board(i,j) is empty iff number of legal values for board(i,j) > 1
 * @param super_array a size^3 array such that super_array[i*size^2 + j*size + v] == 0 iff board(i,j) is empty and v+1 is a legal value for board(i,j)
 * @param dictionary_array a size^3 array such that if X0 in gurobi's variables is in index m in the super_array, dictionary_array[m] = 0
 * @param var_count the number of variables gurobi takes in as input
 * @return 1 iff board is solvable. Function uses ILP in order to solve the board. If value v+1 is the programs choice for board(i,j) in the solution,
 * then super_array[i*size^2 + j*size + v] == 1 at termination.
 */

int gurobi_solve(Board *board, int *super_array, int *dictionary_array, int var_count, int mode);
int get_super_index(int i, int j, int v, int size);
int get_gurobi_index(int i, int j, int v, int size, int* dic_array);
#endif
