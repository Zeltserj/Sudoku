/**
This module communicates with the gurobi optimizer in order to solve a n*m*m*n Sudoku board using ILP or at least making strong guess using LP.
 In both cases, a super array of length (mn)^3 is used in order to index variables.

 **/

/**
 * LP explained:
 * the variable X(i,j,k) corresponds to value k in cell(i,j) of the game board.
 * the solver minimizes the number of variables given to the optimizer (eliminating obvious values, etc.),
 * therefore a mapping is used to map an index in the list of variables back to X(i,j,k)
 *
 * objective function, in LP only:
 * minimize sum(c(i,j,k)*x(i,j,k))
 * where x(i,j,k) is the variable as explained above and c(i,j,k) is its coefficient
 *
 * c(i,j,k) is computed by multiplying the number of possible solutions in cell(i,j) by 10 and adding a random number between 0 and 9
 * thus, we achieve both a degree of randomness (adding a random offset benefits variables with lower random offsets) and a preference for more probable values
 * trying to minimize will cause the optimizer to give higher values (between 0 and 1) to variables with lower coefficients, which are more likely to be placed
 * in a certain cell
 */

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
 * @param gurobi_mode == 1 iff gurobi is to solve the board using ILP
 * @return 1 iff board is solvable. Function uses ILP in order to solve the board. If value v+1 is the programs choice for board(i,j) in the solution,
 * then super_array[i*size^2 + j*size + v] == 1 at termination.
 */

int gurobi_solve(Board *board, double *super_array, int *dictionary_array, int var_count, int gurobi_mode);
int get_super_index(int i, int j, int v, int size);
int get_gurobi_index(int i, int j, int v, int size, int* dic_array);
#endif
