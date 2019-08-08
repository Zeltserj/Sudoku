

#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "execute.h"
#include "solver.h"


void set_LP_type(double *objective, char *vtype, int count, int is_binary);


int set_cell_constraints(GRBmodel *model, int *super_array, int size, int *dic_array);

int set_row_constraints(GRBmodel *model, int *super_array, int size, int *dic_array);

int set_col_constraints(GRBmodel *model, int *super_array, int size, int *dic_array);

int set_block_constraints(GRBmodel *model, int *super_array, int size, int *dic_array, int rows, int cols);

int generate_block_constraints(GRBmodel *model, int size, int row_start, int col_start, int rows, int cols,
                               int *super_array,
                               int *dic_array);

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

int get_super_index(int i, int j, int v, int size) {
    return i * size * size + j * size + v;
}

int get_gurobi_index(int i, int j, int v, int size, int *dic_array) {
    return dic_array[get_super_index(i, j, v, size)];
}
/*TODO: Or: moved  get_all_sol_cell to execute, to avoid include*/

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its row
*/
int validate_cell_row(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int i, num_errors = 0, is_changed = 0;
    Cell *cell_cpy;
    for (i = 0; i < get_size(board); i++) {
        if (i != c && get(board, r, i) == value) {
            if (!is_fixed(board, r, i)) {
                cell_cpy = get_cell_cpy(board, r, i);
                if (mark_errors == 1)
                    is_changed = increase_error(board, r, i);
                else if (mark_errors == -1)
                    is_changed = decrease_error(board, r, i);

                if (mark_errors != 0 && is_changed)
                    add_cell_after_curr(changed_cells, cell_cpy);
                else
                    free(cell_cpy);
            }
            num_errors++;
        }
    }
    return num_errors;
}

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its column
*/
int validate_cell_col(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int i, num_errors = 0, is_changed = 0;
    Cell *cell_cpy;
    for (i = 0; i < get_size(board); i++) {
        if (i != r && get(board, i, c) == value) {
            if (!is_fixed(board, i, c)) {
                cell_cpy = get_cell_cpy(board, i, c);
                if (mark_errors == 1)
                    is_changed = increase_error(board, i, c);
                else if (mark_errors == -1)
                    is_changed = decrease_error(board, i, c);

                if (mark_errors != 0 && is_changed)
                    add_cell_after_curr(changed_cells, cell_cpy);
                else
                    free(cell_cpy);
            }
            num_errors++;
        }
    }
    return num_errors;
}

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param mark_errors: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its block.
*/
int validate_cell_block(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int mark_errors) {
    int first_r, first_c, i, j, num_errors = 0, is_changed = 0;
    int block_r = get_block_rows(board);
    int block_c = get_block_cols(board);
    Cell *cell_cpy;

    first_r = (r / block_r) * block_r;
    first_c = (c / block_c) * block_c;
    for (i = first_r; i < first_r + block_r; i++) {
        for (j = first_c; j < first_c + block_c; j++) {
            if ((i != r && j != c) && get(board, i, j) == value) {
                if (!is_fixed(board, i, j)) {
                    cell_cpy = get_cell_cpy(board, i, j);
                    if (mark_errors == 1)
                        is_changed = increase_error(board, i, j);
                    else if (mark_errors == -1)
                        is_changed = decrease_error(board, i, j);

                    if (mark_errors != 0 &&
                        is_changed) { /*row & col conflicts are handled by validate_cell_row/cell. */
                        add_cell_after_curr(changed_cells, cell_cpy);
                    } else
                        free(cell_cpy);
                }
                num_errors++;
            }
        }
    }
    return num_errors;
}

int validate_cell(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int inc_or_dec) {
    int num_errors, is_valid = 0;
    int num_err_row, num_err_col, num_err_block;
    num_err_row = validate_cell_row(board, changed_cells, r, c, value, inc_or_dec);
    num_err_col = validate_cell_col(board, changed_cells, r, c, value, inc_or_dec);
    num_err_block = validate_cell_block(board, changed_cells, r, c, value, inc_or_dec);
    num_errors = num_err_row + num_err_col + num_err_block;
    if (num_errors == 0)
        is_valid = 1;
    if (inc_or_dec != 0) {
        while (num_errors != 0) {
            if (inc_or_dec > 0)
                increase_error(board, r, c);
            else
                decrease_error(board, r, c);
            num_errors--;
        }
    }
    return is_valid;
}

int *get_all_sol_cell(Board *board, int r, int c) {
    int *sol = calloc(get_size(board), sizeof(int));
    int i, num_sol = 0;
    if (sol == NULL) {
        error("execute", "get_all_sol_cell", 1);
        exit(0);
    }
    for (i = 0; i < get_size(board); i++) {
        if (validate_cell(board, NULL, r, c, i + 1, 0)) {
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
int generate_variable_array(Board *board, int *super_array, int *dic_array) {
    int dim = get_size(board), i, j, v, var_count = 0;
    int *solution_for_cell = NULL;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            /*optimization: get the solution array for empty cells only*/
            solution_for_cell = get_all_sol_cell(board, i, j);
            for (v = 0; v < dim; v++) {
                if (get(board, i, j) == 0 && solution_for_cell != NULL && solution_for_cell[v] == 1) {
                    super_array[get_super_index(i, j, v, dim)] = 0;
                    dic_array[get_super_index(i, j, v, dim)] = var_count;
                    var_count++;}
                else {
                    super_array[get_super_index(i, j, v, dim)] = -1;
                    dic_array[get_super_index(i, j, v, dim)] = -1;
                }    /*a non empty cell gets value or illegal value set to -1 in super array*/
            }
        }

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
void filter_variables(int *super_array, double *sol_array, int size) {
    int i, inner = 0;
    for (i = 0; i < size * size * size; i++) {
        if (super_array[i] == 0) {
            super_array[i] = (int)sol_array[inner];
            inner++;
        }
    }
}

int ILP_solve(Board *board, int *super_array) {
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    int size = get_size(board), _error = 0, var_count, optimstatus;
    double *objective, *var_arr;
    char *vtype;
    int *dictionary_array = calloc(size * size * size, sizeof(int));

    if (super_array == NULL || dictionary_array == NULL) {
        error("solver", "ILP_solve", 1);
        exit(0);
    }
    var_count = generate_variable_array(board, super_array, dictionary_array);

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

    _error = GRBwrite(model,"sudoku.lp");
    if(_error){gurobi_error(_error,env);}

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

    _error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if(_error){ gurobi_error(_error,env);}

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

int generate_solution(Board* board){
    int size = get_size(board), i, j, v,solved;
    int* solution = calloc(size*size*size, sizeof(int));
    solved = ILP_solve(board, solution);
    if(solved){
        for(i = 0; i < size; i ++){
            for(j= 0; j < size; j++){
                if(get(board,i,j) == 0){
                    for( v = 0; v< size; v++){
                        if(solution[get_super_index(i,j,v,size)]==1){
                            set_value(board,i,j,v+1);
                        }
                    }
                }
            }
        }
    return 1;}
    else{printf("unsolvable\n");}
    return 0;
}