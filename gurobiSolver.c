
#include <stdlib.h>
#include "gurobiSolver.h"
#include "gurobi_c.h"



/**
 *
 * @param i
 * @param j
 * @param v
 * @param size
 * @return the index in the super_array corresponding to X(i,j,v)
 */
int get_super_index(int i, int j, int v, int size) {
    return i * size * size + j * size + v;
}

/**
 *
 * @param i
 * @param j
 * @param v
 * @param size
 * @param dic_array
 * @return the index of variable X(i,j,v) in the variable array gurobi was given as input
 */
int get_gurobi_index(int i, int j, int v, int size, int *dic_array) {
    return dic_array[get_super_index(i, j, v, size)];
}

void filter_variables(double *super_array, double *sol_array, int size);

int set_cell_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int *ind, double *val);

int set_row_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int *ind, double *val);

int set_col_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int *ind, double *val);

int set_block_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int rows, int cols, int *ind,
                          double *val);

int generate_block_constraints(GRBmodel *model, int size, int row_start, int col_start, int rows, int cols,
                               double *super_array, int *dic_array, int *ind, double *val);

int set_objective(GRBmodel *model, double *super_array, int *dictionary_array, int var_count, int size, int mode,
                  char *vtype, double *objective);

double set_coeff(double *super_array, int i, int j, int size);

int set_range_constraints(GRBmodel *model, int var_count);

int gurobi_solve(Board *board, double *super_array, int *dictionary_array, int var_count, int gurobi_mode) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    int optimstatus, _error, size = get_size(board);
    double  *var_arr = (double*)calloc(var_count, sizeof(double));
    double* val = (double*)calloc(size,sizeof(double));
    double* objective = (double*)calloc(var_count, sizeof(double));
    int* ind = (int*)calloc(size, sizeof(int));
    char* vtype = (char*)calloc(var_count, sizeof(char));
     if (var_arr == NULL || ind == NULL || vtype == NULL || objective == NULL || val == NULL) {
         error("gurobiSolver","gurobi_solve",1);
        exit(0);
    }
    _error = GRBloadenv(&env, "sudoku.log");
    if (_error) { gurobi_error(_error, env); }

    _error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (_error) { gurobi_error(_error, env); }

    _error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL);
    if (_error) { gurobi_error(_error, env); }


    _error = set_objective(model, super_array, dictionary_array, var_count, size, gurobi_mode, vtype, objective);
    if (_error) { gurobi_error(_error, env); }

    _error = GRBupdatemodel(model);
    if (_error) { gurobi_error(_error, env); }

    if(gurobi_mode != ILP){
        _error = set_range_constraints(model, var_count);
        if(_error){ gurobi_error(_error, env);}
    }
    _error = set_cell_constraints(model, super_array, size, dictionary_array, ind, val);
    if (_error) { gurobi_error(_error, env); }

    _error = set_row_constraints(model, super_array, size, dictionary_array, ind, val);
    if (_error) { gurobi_error(_error, env); }

    _error = set_col_constraints(model, super_array, size, dictionary_array, ind, val);
    if (_error) { gurobi_error(_error, env); }


    _error = set_block_constraints(model, super_array, size, dictionary_array, get_block_rows(board),
                                   get_block_cols(board), ind, val);
    if (_error) { gurobi_error(_error, env); }

    _error = GRBoptimize(model);
    if (_error) { gurobi_error(_error, env); }

   _error = GRBwrite(model, "sudokuProgram.lp");
    if (_error) { gurobi_error(_error, env); }

    _error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (_error) { gurobi_error(_error, env); }

    if (optimstatus != GRB_OPTIMAL) {
        free(var_arr);
        free(objective);
        free(vtype);
        free(ind);
        free(val);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return 0;
    }

    _error = GRBwrite(model, "sudoku.sol");
    if (_error) { gurobi_error(_error, env); }


    _error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, var_count, var_arr);
    if (_error) { gurobi_error(_error, env); }

    filter_variables(super_array, var_arr, size);
    free(var_arr);
    free(objective);
    free(vtype);
    free(ind);
    free(val);
    GRBfreemodel(model);
    GRBfreeenv(env);

    return 1;

}

/**
 *
 * @param model working gurobi Model
 * @param var_count number of variables in the model
 * @return 0 iff range constraints were added successfully
 */
int set_range_constraints(GRBmodel *model, int var_count) {
    int i, _error = 0;
    int ind[1];
    double val[1];
    for (i = 0; i < var_count; i++) {
        ind[0] = i;
        val[0] = 1.0;
        _error = GRBaddconstr(model, 1, ind, val, GRB_LESS_EQUAL, 1, NULL);
        if (_error)
            return _error;
        _error = GRBaddconstr(model, 1, ind, val, GRB_GREATER_EQUAL, 0, NULL);
    }
    if (_error)
        return _error;
    return 0;
}

/**
 *
 * @param model Gurobi model
 * @param super_array an array of length size^3
 * @param dictionary_array the mapping between the index in gurobi's input and the super_array
 * @param var_count number of variables in the gurobi
 * @param size == get_size(board) of current board
 * @param mode == LP || mode == ILP
 * @param vtype empty array for gurobi's vtype (binary or continuous)
 * @param objective empty array to insert coefficients as explained in header
 * @return 0 iff objective functions was successfully added to the model
 */
int set_objective(GRBmodel *model, double *super_array, int *dictionary_array, int var_count, int size, int mode,
                  char *vtype, double *objective) {
    int i, j, v, index, _error;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++){
            for(v=0; v< size; v++){
                if(super_array[get_super_index(i,j,v,size)] == 0){
                    index = get_gurobi_index(i,j,v,size,dictionary_array);
                    if(mode==ILP){
                        objective[index] = 0.0;
                        vtype[index] = GRB_BINARY;
                    }
                    else{
                        objective[index] = set_coeff(super_array,i,j,size);
                        vtype[index] = GRB_CONTINUOUS;
                    }
                }
            }
        }
    }
    _error = GRBaddvars(model, var_count, 0, NULL,NULL,NULL,objective,NULL,NULL,vtype,NULL);
    return _error;
}

/**
 *
 * @param super_array an array of length size^3 where super_array[get_super_index(i,j,v,size)] == 0 corresponds to a variable
 * @param i rows index of variable
 * @param j col index of variable
 * @param size == get_size(board) of current game board
 * @return sets the coefficient of x(i,j,k) if it is a relevant variable as explained in header file
 */
double set_coeff(double *super_array, int i, int j, int size) {
    int offset = rand() %10;
    int v, count = 0;
    double out;
    for(v=0;v<size;v++){
        if(super_array[get_super_index(i,j,v,size)] == 0){
            count++;
        }
    }
    out = (double)(10*count+offset);
    return out;
}

/**
 *
 * @param model Gurobi model
 * @param super_array an array of length size^3
 * @param size = m*n where m , n are the columns, rows per block of the board the module is solving respectively
 * @param dic_array the mapping between the index in gurobi's input and the super_array
 * @ind array for indices
 * @val array for values
 * @return 0 if successful and all column constraints were added to the model (single value in each column), or error code of gurobi.
 */

int set_col_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int *ind, double *val) {
    int i, j, v, size_count = 0, _error = 0;

    for (v = 0; v < size; v++) {
        for (j = 0; j < size; j++) {
            for (i = 0; i < size; i++) {
                /*this means this X(i,j,v) is a variable*/
                if (super_array[get_super_index(i, j, v, size)] == 0) {
                    ind[size_count] = get_gurobi_index(i, j, v, size, dic_array);
                    val[size_count] = 1.0;
                    size_count++;
                }
            }
            if (size_count > 0) {
                _error = GRBaddconstr(model, size_count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (_error) {
                    return _error;
                }
            }
            size_count = 0;
        }

    }
    return 0;
}


/**
 *
 * @param model Gurobi model
 * @param super_array an array of length size^3
 * @param size = m*n where m , n are the columns, rows per block of the board the module is solving respectively
 * @param dic_array the mapping between the index in gurobi's input and the super_array
 * @ind array for indices
 * @val array for values
 * @return 0 if successful and all cell constraints were added to the model (each cell holds one value), or error code of gurobi.
 */


int set_cell_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int *ind, double *val) {
    int i, j, v, size_count = 0, _error = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            for (v = 0; v < size; v++) {
                /*this means this X(i,j,v) is a variable in gurobi's input*/
                if (super_array[get_super_index(i, j, v, size)] == 0) {
                    ind[size_count] = get_gurobi_index(i, j, v, size, dic_array);
                    val[size_count] = 1.0;
                    size_count++;
                }

            }

            if (size_count > 0) {
                _error = GRBaddconstr(model, size_count, ind, val, GRB_EQUAL, 1, NULL);
                if (_error) { return _error; }
                size_count = 0;

            }
        }
    }

    return 0;
}

/**
 *
 * @param model Gurobi model
 * @param super_array an array of length size^3
 * @param size = m*n where m , n are the columns, rows per block of the board the module is solving respectively
 * @param dic_array the mapping between the index in gurobi's input and the super_array
 * @ind array for indices
 * @val array for values
 * @return 0 if successful and all row constraints were added to the model (single value in each row), or error code of gurobi.
 */

int set_row_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int *ind, double *val) {
    int i, j, v, size_count = 0, _error = 0;
    for (v = 0; v < size; v++) {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                /*this means this X(i,j,v) is a variable*/
                if (super_array[get_super_index(i, j, v, size)] == 0) {
                    ind[size_count] = get_gurobi_index(i, j, v, size, dic_array);
                    val[size_count] = 1.0;
                    size_count++;
                }
            }
            if (size_count > 0) {
                _error = GRBaddconstr(model, size_count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (_error) {
                    error("solver", "set row constraints", 29);
                    return _error;
                }
            }
            size_count = 0;
        }

    }
    return 0;
}

/**
 *
 * @param model Gurobi model
 * @param super_array an array of length size^3 with -1 corresponding to variables not included in the gurobi model
 * @param size = m*n where m , n are the columns, rows per block of the board the module is solving respectively
 * @param dic_array the mapping between the index in gurobi's input and the super_array
 * @param rows number of rows in each block (n)
 * @param cols number of columns in each block (m)
 * @ind array for indices
 * @val array for values
 * @return 0 if successful and all block constraints were added to the model (single value in each block), or error code of gurobi.
 */


int set_block_constraints(GRBmodel *model, double *super_array, int size, int *dic_array, int rows, int cols, int *ind,
                          double *val) {
    int i, j, _error = 0;
    for (i = 0; i < size; i += rows) {
        for (j = 0; j < size; j += cols) {
            _error = generate_block_constraints(model, size, i, j, rows, cols, super_array, dic_array, ind, val);
            if (_error) {
                return _error;
            }

        }
    }
    return 0;
}

/**
 *
 * @param model a working gurobi model
 * @param size = rows*cols
 * @param row_start index of the row where the block starts, going from left to right, up to down
 * @param col_start index of the column where the block starts, going from left to right, up to down
 * @param rows number of rows in block (n)
 * @param cols number of columns in block (m)
 * @param super_array an array of length size^3 with -1 corresponding to variables not included in the gurobi model
 * @param dic_array the mapping between the index in gurobi's input and the super_array
 * @ind array for indices
 * @val array for values
 * @return 0 if successful and all block constraints of block starting at (row_start,col_start) were added to the model
 * (each value appears once in the block), or error code of gurobi.
 */
int generate_block_constraints(GRBmodel *model, int size, int row_start, int col_start, int rows, int cols,
                               double *super_array, int *dic_array, int *ind, double *val) {
    int i, j, v, size_count = 0, _error = 0;
    for (v = 0; v < size; v++) {
        for (i = row_start; i < row_start + rows; i++) {
            for (j = col_start; j < col_start + cols; j++) {
                if (super_array[get_super_index(i, j, v, size)]==0) {
                    ind[size_count] = get_gurobi_index(i, j, v, size, dic_array);
                    val[size_count] = 1.0;
                    size_count++;
                }
            }
			
           
        }
		 if (size_count > 0) {
                _error = GRBaddconstr(model, size_count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (_error) {
                    error("solver", "generate block constraints", 29);
                    return 1;
                }
            }
            size_count = 0;
        
    }
    return 0;
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

void filter_variables(double *super_array, double *sol_array, int size) {
    int i, inner = 0;
    for (i = 0; i < size * size * size; i++) {
        if (super_array[i] == 0) {
            super_array[i] = sol_array[inner];
            inner++;
        }
    }
}