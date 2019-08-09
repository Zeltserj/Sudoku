/**

 Created by lenovo on ${DTE}
 **/

#include <stdlib.h>
#include "gurobiSolver.h"
#include "gurobi_c.h"
int get_super_index(int i, int j, int v, int size) {
    return i * size * size + j * size + v;
}

int get_gurobi_index(int i, int j, int v, int size, int *dic_array) {
    return dic_array[get_super_index(i, j, v, size)];
}

void set_LP_type(double *objective, char *vtype, int count, int is_binary);

void filter_variables(int *super_array, double *sol_array, int size);
int set_cell_constraints(GRBmodel *model, int *super_array, int size, int *dic_array);
int set_row_constraints(GRBmodel *model, int *super_array, int size, int *dic_array);
int set_col_constraints(GRBmodel *model, int *super_array, int size, int *dic_array);
int set_block_constraints(GRBmodel *model, int *super_array, int size, int *dic_array, int rows, int cols);
int generate_block_constraints(GRBmodel *model, int size, int row_start, int col_start, int rows, int cols,
                               int *super_array,
                               int *dic_array);

int gurobi_solve_ILP(Board *board, int *super_array, int *dictionary_array, int var_count) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    int optimstatus, _error, size = get_size(board);
    char *vtype;
    double *objective, *var_arr;
    var_arr = calloc(var_count, sizeof(double));
    objective = calloc(var_count, sizeof(double));
    vtype = calloc(var_count, sizeof(char));


    if (vtype == NULL || objective == NULL) {
        error("solver", "ILP_solve", 1);
        exit(0);
    }
    _error = GRBloadenv(&env, "sudoku.log");
    if (_error) { gurobi_error(_error, env); }

    _error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
    if (_error) { gurobi_error(_error, env); }

    _error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL);
    if (_error) { gurobi_error(_error, env); }


    set_LP_type(objective, vtype, var_count, 1);
    _error = GRBaddvars(model, var_count, 0, NULL, NULL, NULL, objective, NULL, NULL, vtype, NULL);
    if (_error) { gurobi_error(_error, env); }

    _error = GRBupdatemodel(model);
    if (_error) { gurobi_error(_error, env); }

    _error = set_cell_constraints(model, super_array, size, dictionary_array);


    if (_error) { gurobi_error(_error, env); }
    /*message was printed in the function*/
    _error = set_row_constraints(model, super_array, size, dictionary_array);
    if (_error) { gurobi_error(_error, env); }

    _error = set_col_constraints(model, super_array, size, dictionary_array);
    if (_error) { gurobi_error(_error, env); }


    _error = set_block_constraints(model, super_array, size, dictionary_array, get_block_rows(board),
                                   get_block_cols(board));
    if (_error) { gurobi_error(_error, env); }

    _error = GRBoptimize(model);
    if (_error) { gurobi_error(_error, env); }

    _error = GRBwrite(model,"sudokuWithAutofill.lp");
    if(_error){gurobi_error(_error,env);}

    _error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if(_error){ gurobi_error(_error,env);}

    if(optimstatus != GRB_OPTIMAL){
        free(var_arr);
        free(dictionary_array);
        free(vtype);
        free(objective);
        GRBfreemodel(model);
        GRBfreeenv(env);
        return 0;
    }

    _error = GRBwrite(model,"sudoku.sol");
    if(_error){gurobi_error(_error,env);}


    _error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, var_count, var_arr);
    if(_error){gurobi_error(_error,env);}

    filter_variables(super_array, var_arr,size);

    free(var_arr);
    free(dictionary_array);
    free(vtype);
    free(objective);
    GRBfreemodel(model);
    GRBfreeenv(env);
    return 1;

}


int set_col_constraints(GRBmodel *model, int *super_array, int size, int *dic_array) {
    int i, j, v, size_count = 0, _error = 0;
    int ind[size];
    double val[size];
    for (v = 0; v < size; v++) {
        for (j = 0; j < size; j++) {
            for (i = 0; i < size; i++) {
                /*this means this X(i,j,v) is a variable*/
                if (super_array[get_super_index(i, j, v, size)] == 0) {
                    ind[size_count] = get_gurobi_index(i,j,v,size,dic_array);
                    val[size_count] = 1.0;
                    size_count++;
                }
            }
            if (size_count > 0) {
                _error = GRBaddconstr(model, size_count, ind, val, GRB_EQUAL, 1.0, NULL);
                if (_error) {
                    error("solver", "set col constraints", 29);
                    return 1;
                }
            }
            size_count = 0;
        }

    }
    return 0;
}

void set_LP_type(double *objective, char *vtype, int count, int is_binary) {
    int i;
    for (i = 0; i < count; i++) {
        if (is_binary) {
            vtype[i] = GRB_BINARY;
            objective[i] = 0.0;
        } else {
            vtype[i] = GRB_CONTINUOUS;
            /*TODO: this needs to change according to objective function*/
            objective[i] = 0.0;
        }
    }
}

int set_cell_constraints(GRBmodel *model, int *super_array, int size, int *dic_array) {
    int i, j, v, size_count = 0, _error = 0;
    int ind[size];
    double val[size];
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            for (v = 0; v < size; v++) {
                /*this means this X(i,j,v) is a variable*/
                if (super_array[get_super_index(i, j, v, size)] == 0) {
                    ind[size_count] = get_gurobi_index(i,j,v,size,dic_array);
                    val[size_count] = 1.0;
                    size_count++;
                }

            }

            if (size_count > 0) {
                _error = GRBaddconstr(model, size_count, ind, val, GRB_EQUAL, 1, NULL);
                if (_error) {return _error;}
                size_count = 0;

            }
        }
    }

    return 0;
}


int set_row_constraints(GRBmodel *model, int *super_array, int size, int *dic_array) {
    int i, j, v, size_count = 0, _error = 0;
    int ind[size];
    double val[size];
    for (v = 0; v < size; v++) {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                /*this means this X(i,j,v) is a variable*/
                if (super_array[get_super_index(i, j, v, size)] == 0) {
                    ind[size_count] = get_gurobi_index(i,j,v,size,dic_array);
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




int set_block_constraints(GRBmodel *model, int *super_array, int size, int *dic_array, int rows, int cols) {
    int i, j, _error = 0;
    for (i = 0; i < size; i += rows) {
        for (j = 0; j < size; j += cols) {
            _error = generate_block_constraints(model, size, i, j, rows, cols, super_array, dic_array);
            if(_error){
                return _error;
            }

        }
    }
    return 0;
}

int generate_block_constraints(GRBmodel *model, int size, int row_start, int col_start, int rows, int cols,
                               int *super_array,
                               int *dic_array) {
    int i, j, v, size_count = 0, _error = 0;
    int ind[size];
    double val[size];
    for (v = 0; v < size; v++) {
        for (i = row_start; i < row_start + rows; i++) {
            for (j = col_start; j < col_start + cols; j++){
                if(super_array[get_super_index(i,j,v,size) == 0]){
                    ind[size_count] = get_gurobi_index(i,j,v,size,dic_array);
                    val[size_count] = 1.0;
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
    }
    return 0;
}

void filter_variables(int *super_array, double *sol_array, int size) {
    int i, inner = 0;
    for (i = 0; i < size * size * size; i++) {
        if (super_array[i] == 0) {
            super_array[i] = (int)sol_array[inner];
            inner++;
        }
    }
}