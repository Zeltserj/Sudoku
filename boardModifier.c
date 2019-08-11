

#include <stdlib.h>
#include <stdio.h>
#include "execute.h"
#include "boardModifier.h"
#include "linkedList.h"
#include "gurobiSolver.h"

int get_next_cell(Board *board, int r, int c, int *next_cell);
int set_first_cell(Board *brd_cpy, Stack *stack, int *next_cell);
void print_int_arr(int* arr, int len) {
    int i;
    for (i = 0; i < len; i++)
        printf("arr[%d] = %d\n", i, (arr[i]));
}
/**
 *
 * @param size = board dimension
 * @return matrix of size*size
 */

int **allocate_int_matrix(int size) {
    int **matrix = (int **) calloc(size, sizeof(int *));
    int i;
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

/**
* @param board != NULL
* @param changed_cells != NULL pointer to the list of the changed cells of the command
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its row
*/
int validate_cell_row(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int inc_or_dec) {
    int i, num_errors = 0, is_changed = 0;
    Cell *cell_cpy;
    for (i = 0; i < get_size(board); i++) {
        if (i != c && get(board, r, i) == value && value!= 0) {
            if (!is_fixed(board, r, i)) {
                cell_cpy = get_cell_cpy(board, r, i);
                if (inc_or_dec == 1)
                    is_changed = increase_error(board, r, i);
                else if (inc_or_dec == -1)
                    is_changed = decrease_error(board, r, i);

                if (inc_or_dec != 0 && is_changed)
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
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its column
*/
int validate_cell_col(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int inc_or_dec) {
    int i, num_errors = 0, is_changed = 0;
    Cell *cell_cpy;
    for (i = 0; i < get_size(board); i++) {
        if (i != r && get(board, i, c) == value && value!= 0) {
            if (!is_fixed(board, i, c)) {
                cell_cpy = get_cell_cpy(board, i, c);
                if (inc_or_dec == 1)
                    is_changed = increase_error(board, i, c);
                else if (inc_or_dec == -1)
                    is_changed = decrease_error(board, i, c);

                if (inc_or_dec != 0 && is_changed)
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
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its block.
*/
int validate_cell_block(Board *board, LinkedListCells *changed_cells, int r, int c, int value, int inc_or_dec) {
    int first_r, first_c, i, j, num_errors = 0, is_changed = 0;
    int block_r = get_block_rows(board);
    int block_c = get_block_cols(board);
    Cell *cell_cpy;

    first_r = (r / block_r) * block_r;
    first_c = (c / block_c) * block_c;
    for (i = first_r; i < first_r + block_r; i++) {
        for (j = first_c; j < first_c + block_c; j++) {
            if ((i != r && j != c) && get(board, i, j) == value && value!= 0) {
                if (!is_fixed(board, i, j)) {
                    cell_cpy = get_cell_cpy(board, i, j);
                    if (inc_or_dec == 1)
                        is_changed = increase_error(board, i, j);
                    else if (inc_or_dec == -1)
                        is_changed = decrease_error(board, i, j);

                    if (inc_or_dec != 0 && is_changed) { /*row & col conflicts are handled by validate_cell_row/cell. */
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

int * get_all_sol_cell(Board *board, int r, int c) {
    int i, num_sol = 0, len=get_size(board);
    int *sol = (int*)calloc(len ,sizeof(int));
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
    return sol;
}

/**
 *
 * @param board is valid (is_erroneous(board) == 0)
 * @return number of variables to be sent to gurobi also sets the super array: an array A of size size^3 + 1, where A(i*size^2+j*size+k) == 0 if board(i,j) is filled or k+1 is not a legal value for board(i,j)
 * A(i*size^2+j*size+k) == 1 iff k+1 is a possible solution for the empty cell board(i,j)
 * last value in the array hold number of variables to be sent to gurobi
 */
int generate_variable_array(Board *board, double *super_array, int *dic_array) {
    int dim = get_size(board), i, j, v, var_count = 0;
    int *solution_for_cell = NULL;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            /*optimization: get the solution array for empty cells only*/
            solution_for_cell = get_all_sol_cell(board, i, j);
            for (v = 0; v < dim; v++) {
                if (get(board, i, j) == 0 && solution_for_cell[v] == 1) {
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



int solve(Board *board, double *super_array, int gurobi_mode) {
    int size = get_size(board), var_count, autofills;
    int *dictionary_array = (int*)calloc(size * size * size, sizeof(int));
    if (super_array == NULL || dictionary_array == NULL) {
        error("solver", "solve", 1);
        exit(0);
    }
    autofills = autofill(board,NULL);
    while(autofills > 0){
        autofills = autofill(board, NULL);
    }

    var_count = generate_variable_array(board, super_array, dictionary_array);
    return gurobi_solve(board, super_array, dictionary_array, var_count, gurobi_mode);
}

int autofill(Board* board, LinkedList *moves){
    Board* b_cpy = brdcpy(board);
    int i,j,v, count = 0;
    for(i=0; i<get_size(board);i++){
        for(j=0;j<get_size(board);j++){
            if(get(b_cpy,i,j) == 0) {
                v = get_single_value(b_cpy, i, j);
                if (v != 0) {
                    if(moves != NULL)
                        set_command(board, moves, i, j, v);
                    else
                        set_value(board,i,j,v);
                count++;
                }
            }
        }
    }
    free_board(b_cpy);
    return count;
}

int generate_solution(Board *board, int fill_board) {
    int size = get_size(board), i, j, v, solved;
    double *solution = (double *) calloc(size * size * size, sizeof(double));
    solved = solve(board, solution, ILP);
    if (solved) {
        if(fill_board) {
            for (i = 0; i < size; i++) {
                for (j = 0; j < size; j++) {
                    if (get(board, i, j) == 0) {
                        for (v = 0; v < size; v++) {
                            if (solution[get_super_index(i, j, v, size)] == 1) {
                                set_value(board, i, j, v + 1);
                            }
                        }
                    }
                }
            }
        }
        free(solution);
        return 1;
    }
    free(solution);
    return 0;
}
void change_cells_to(Board *board, LinkedListCells *old_values) {
    int i, len = get_len_linked_list_cells(old_values);
    Cell* curr;
    move_curr_to_head(old_values);
    for(i=0; i < len; i++){
        curr=get_curr_cell(old_values);
        set_cell(board,curr);
        advance_curr_cell(old_values);
    }
    move_curr_to_head(old_values);
}

void set_command(Board *board, LinkedList *moves, int r, int c, int value) {
    Node* curr = get_curr(moves);
    LinkedListCells* curr_changed = get_changed_cells_list(curr);
    int prev_value=get(board,r,c);
    if(value!= prev_value)
    {
        add_cell_after_curr(curr_changed,get_cell_cpy(board,r,c));
        if(is_error(board,r,c))
            validate_cell(board, curr_changed, r, c, prev_value,-1);
        validate_cell(board,curr_changed,r,c,value,1);
        set_value(board,r,c,value);
    }
}

int num_solutions_BT(Board *board) {
    Stack *stack = alloc_stack();
    StackNode* curr_node;
    Board* brd_cpy = brdcpy(board);
    int *next_cell = (int*)calloc(2, sizeof(int)), *possible_sols_cell;
    int sol_count = 0, next_r =0,next_c =0, i=0, len = get_size(brd_cpy), exist_next;
    if (next_cell == NULL) {
        error("boardModifier","num_solutions_BT",1);
        exit(0);
    }
    if(set_first_cell(brd_cpy, stack, next_cell) == 0) /*finds the first empty cell to start from and push it to the stack*/
        return 0;
    while (size_stack(stack) != 0){
        curr_node = peek(stack);
        i = get_value_stack_node(curr_node);
        possible_sols_cell = get_possible_sols(curr_node);
        while (i < len && possible_sols_cell[i] != 1)
            i++;
        if(i<len){
            exist_next =1 ;
            set_value_stack_node(curr_node, i+1);
            set_value(brd_cpy,get_row_stack_node(curr_node),get_col_stack_node(curr_node),i+1);
            while(exist_next){
                exist_next = get_next_cell(brd_cpy, get_row_stack_node(curr_node), get_col_stack_node(curr_node), next_cell);
                if (exist_next) {
                    next_r = next_cell[0];
                    next_c = next_cell[1];
                    possible_sols_cell = get_all_sol_cell(brd_cpy, next_r, next_c);
                    i=0;
                    while (i < len && possible_sols_cell[i]!= 1)
                        i++;
                    if(i!=len){
                        i++;
                        set_value(brd_cpy,next_r,next_c,i);
                        push(stack,next_r,next_c,i);
                        curr_node = peek(stack);
                        set_possible_sols(curr_node,possible_sols_cell);
                    }
                    else
                        exist_next =0;
                }
            }
        }
        else {
            curr_node = pop(stack);
            set_value(brd_cpy,get_row_stack_node(curr_node),get_col_stack_node(curr_node),0);
            free_stack_node(curr_node);
        }
        if(get_num_empty(brd_cpy) == 0) {
            sol_count++;
            /*print_board(brd_cpy);*/
        }
    }
    free(next_cell);
    free_board(brd_cpy);
    return sol_count;
}

int get_next_cell(Board *board, int r, int c, int *next_cell) {
    int next_r = r, next_c = c, len=get_size(board);
    if (c == len) {
        if (r == len) {
            if (get(board, next_r, next_c) != 0)
                return 0;
            next_c = 0;
            next_r++;
        }
    } else {
        next_c++;
    }
    while (next_r < len) {
        while (next_c < len) {
            if (get(board, next_r, next_c) == 0) {
                next_cell[1] = next_c;
                next_cell[0] = next_r;
                return 1;
            }
            next_c++;
        }
        next_c = 0;
        next_r++;
    }
    return 0;
}

int set_first_cell(Board *brd_cpy, Stack *stack, int *next_cell) {
    int next_r =0, next_c=0, i=0, exist_next;
    int *possible_sols_cell;

    if (get(brd_cpy, 0, 0) != 0) {
        exist_next = get_next_cell(brd_cpy, 0, 0, next_cell);
        if(!exist_next){ /*means no next empty cell*/
            if(is_erroneous(brd_cpy))
                return 0;
            else
                return 1;
        }
        next_r = next_cell[0];
        next_c = next_cell[1];
    }
    possible_sols_cell = get_all_sol_cell(brd_cpy, next_r, next_c);
    while(i<get_size(brd_cpy) && possible_sols_cell[i] != 1){
        i++;
    }
    if(possible_sols_cell[i] == 1){
        push(stack,next_r,next_c,0);
        set_possible_sols(peek(stack),possible_sols_cell);
        set_value(brd_cpy,next_r,next_c,0);
    }
    return 1;
}

double *get_probability_array(Board *board, double *solution, int i, int j) {
    double* out;
    int v, size = get_size(board),index;
    out = (double*)calloc(size, sizeof(double));
    if(out == NULL){
        error("boardModifier", "get_probability_array", 1);
        exit(0);
    }
    if(get(board,i,j) != 0){
        out[get(board,i,j)-1] = 1.0;
        return out;
    }
    for(v = 0; v < size; v++){
        index = get_super_index(i,j,v,size);
        if(solution[index] != -1){
            out[v] = solution[index];
        }
    }
    return out;
}

int validate_command(Board *board) {
    return generate_solution(board,0);
}

int hint_command(Board *board, int row, int col) {
    Board* brd_cpy = brdcpy(board);
    int hint;
    if(generate_solution(brd_cpy,1)){
        hint = get(brd_cpy,row,col);
        free(brd_cpy);
        return hint;
    }
    return 0;
}

double * guess_hint_command(Board *board, int row, int col) {
    int solved, size = get_size(board);
    double *solution = (double *) calloc(size * size * size, sizeof(double));
    solved = solve(board, solution, ILP);
    if(solved)
        return get_probability_array(board,solution,row,col);
    return NULL;
}

