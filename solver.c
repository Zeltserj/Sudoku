
#include <stdlib.h>
#include <stdio.h>
#include "solver.h"


int **get_cell_const(int count, int size);

/**
 *
 * @param size = board dimension
 * @return matrix of size*size
 */

int **allocate_int_matrix(int size) {
    int **matrix = (int **) calloc(size, sizeof(int *));
    int i, j;
    if (matrix == NULL) {
        error("solver", "allocate_int_matrix", 1);
        exit(0);
    }
    for (i = 0; i < size; i++) {
        matrix[i] = calloc(size, sizeof(int));
        if (matrix[i] == NULL) {
            error("solver", "allocate_int_matrix", 1);
        }
    }
    return matrix;
}

int *get_all_sol_cell(Board *board, int r, int c) {
    int *sol = (int *) calloc(get_size(board), sizeof(int));
    int i, num_sol = 0;
    if (sol == NULL) {
        error("solver", "get_all_sol_cell", 1);
        exit(0);
    }
    for (i = 0; i < get_size(board); i++) {
        if (is_legal_value(board, r, c, i + 1)) {
            sol[i] = 1;
            num_sol++;
        }
    }
    if (num_sol == 0)
        return NULL;
    return sol;
}

/**
 *
 * @param board is valid (is_erroneous(board) == 0)
 * @return number of variables to be sent to gurobi also sets the super array: an array A of size size^3 + 1, where A(i*size^2+j*size+k) == 0 if board(i,j) is filled or k+1 is not a legal value for board(i,j)
 * A(i*size^2+j*size+k) == 1 iff k+1 is a possible solution for the empty cell board(i,j)
 * last value in the array hold number of variables to be sent to gurobi
 */
int generate_variable_array(Board *board, int* var_arr) {
    int dim = get_size(board), i, j, v, var_count = 0;
    int *solution_for_cell = NULL;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            if(get(board, i, j) == 0) /*optimization: get the solution array for empty cells only*/
                solution_for_cell = get_all_sol_cell(board, i, j);
            for (v = 0; v < dim; v++) {
                if (get(board, i, j) == 0) {
                    if (solution_for_cell[v] == 1) {
                        var_arr[var_count] = i * dim * dim + j * dim + v; /* v+1 is a possible solution for out[i][j]*/
                        var_count++;
                    }
                }    /*a non empty cell gets value or illegal value set to -1 in super array*/
            }
        }
    }
    var_arr = realloc(var_arr, var_count* sizeof(int));
    if(var_arr == NULL){
        error("solver", "generate_variable_array",1);
    }
    return var_count;
}

/**
 *
 * @param super_array a size^3+1 array with -1 in irrelevant values (x(i,j,k) not sent to gurobi optimizer) and 0 where variables where sent
 * @param sol_array is the output of the gurobi module
 * @param size size of sol_array
 *
 * the function matches the instances of the solution array with their respective indices in the super array,
 * so they could be accessed with respect to location on the board
 *
 */
void filter_variables(int* super_array, int* sol_array, int size){
    int len = super_array[size*size*size], i, inner = 0;
    int *out = calloc(len, sizeof(int));
    if(out == NULL){
        error("solver", "filter_variables", 1);
        exit(0);
    }
    for(i = 0; i < size*size*size; i++){
        if(super_array[i] == 0){
            super_array[i] = sol_array[inner];
            inner++;
        }
    }
}

int ILP_solve(Board* board){
    int size = get_size(board);
    int *var_arr = calloc(size*size*size, sizeof(int));
    int var_count = generate_variable_array(board,var_arr);



}

int **get_cell_const(int count, int size) {

    return NULL;
}
