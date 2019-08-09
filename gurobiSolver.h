/**

 Created by lenovo on ${DTE}
 **/


#ifndef SP_GUROBISOLVER_H
#define SP_GUROBISOLVER_H

#include "board.h"

int gurobi_solve_ILP(Board *board, int *super_array, int *dictionary_array, int var_count);
int get_super_index(int i, int j, int v, int size);
int get_gurobi_index(int i, int j, int v, int size, int* dic_array);
#endif //SP_GUROBISOLVER_H
