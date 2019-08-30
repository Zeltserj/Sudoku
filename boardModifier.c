

#include <stdlib.h>
#include <stdio.h>
#include "execute.h"
#include "boardModifier.h"
#include "linkedList.h"
#include "gurobiSolver.h"

int get_next_cell(Board *board, int r, int c, int *next_cell);
int set_first_cell(Board *brd_cpy, Stack *stack, int *next_cell);
int generate_board(Board* board, int x, int y);
int fill_x_cells(Board* board, int x);
int set_random_value(Board* board, int row, int col);
void clear_cells(Board* board, int num_keep);
void copy_to_board(Board *to, LinkedList *moves, Board *from);
void fix_y_cells(Board *board, int num_fix);
void unfix_all(Board *board);
int count_sols(int* sol_arr,int len);
int fill_score_arr(int *arr, int len_arr, double *score, int len_score, float threshold);
void fix_prob_arr(double *arr, int *sol_arr, int size);
int get_single_value(Board* board,int r, int c);

/*TODO: delete this debug functions <3 */
void print_int_arr(int * arr, int len) {
    int i;
    for (i = 0; i < len; i++)
        printf("arr[%d] = %d\n", i, (arr[i]));
}
void print_double_arr(double * arr, int len) {
    int i;
    for (i = 0; i < len; i++)
        printf("arr[%d] = %f\n", i, (arr[i]));
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
* @param old_values != NULL pointer to the list of the cell values before the command execution
* @param new_values != NULL pointer to the list of the cell values after the command execution
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its row
*/
int validate_cell_row(Board *board, LinkedListCells *old_values, LinkedListCells *new_values, int r, int c, int value,
                      int inc_or_dec) {
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

                if (inc_or_dec != 0 && is_changed) {
                    add_cell_after_curr(old_values, cell_cpy);
                    add_cell_after_curr(new_values,get_cell_cpy(board,r,i));
                }
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
* @param old_values != NULL pointer to the list of the cell values before the command execution
* @param new_values != NULL pointer to the list of the cell values after the command execution
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its column
*/
int validate_cell_col(Board *board, LinkedListCells *old_values, LinkedListCells *new_values, int r, int c, int value,
                      int inc_or_dec) {
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

                if (inc_or_dec != 0 && is_changed){
                    add_cell_after_curr(old_values, cell_cpy);
                    add_cell_after_curr(new_values,get_cell_cpy(board,i,c));
                }
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
* @param old_values != NULL pointer to the list of the cell values before the command execution
* @param new_values != NULL pointer to the list of the cell values after the command execution
* @param r in range [0,board.size-1]
* @param c in range [0,board.size-1]
* @param value in range [1,board.size]
* @param inc_or_dec: 1 for increasing error for each conflict, -1 for decreasing error for each conflict,
* 0 otherwise.
* @return num of conflicts for cell board[r][c] with its block.
*/
int validate_cell_block(Board *board, LinkedListCells *old_values, LinkedListCells *new_values, int r, int c, int value,
                        int inc_or_dec) {
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
                        add_cell_after_curr(old_values, cell_cpy);
                        add_cell_after_curr(new_values,get_cell_cpy(board,i,j));
                    } else
                        free(cell_cpy);
                }
                num_errors++;
            }
        }
    }
    return num_errors;
}

int validate_cell(Board *board, LinkedListCells *old_values, LinkedListCells *new_values, int r, int c, int value,
                  int inc_or_dec) {
    int num_errors, is_valid = 0;
    int num_err_row, num_err_col, num_err_block;
    num_err_row = validate_cell_row(board, old_values, new_values, r, c, value, inc_or_dec);
    num_err_col = validate_cell_col(board, old_values, new_values, r, c, value, inc_or_dec);
    num_err_block = validate_cell_block(board, old_values, new_values, r, c, value, inc_or_dec);
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
        if (validate_cell(board, NULL, NULL, r, c, i + 1, 0)) {
            sol[i] = 1;
            num_sol++;
        }
    }
    return sol;
}

/**
 *
 * @param board is valid (is_erroneous(board) == 0)
 * @return -1 if there is a cell without a possible solution (therefore board is not solveable).
 * otherwise: number of variables to be sent to gurobi also sets the super array: an array A of size size^3 + 1, where A(i*size^2+j*size+k) == 0 if board(i,j) is filled or k+1 is not a legal value for board(i,j)
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
            if(count_sols(solution_for_cell,dim)==0){
                free(solution_for_cell);
                return -1;
            }
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
            free(solution_for_cell);
        }

    }
    return var_count;
}
int count_sols(int* sol_arr,int len) {
    int i, count = 0;
    for (i = 0; i < len; i++) {
        if (sol_arr[i] == 1)
            count++;
    }
    return count;
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
    if(var_count == -1)
        return 0;
    return gurobi_solve(board, super_array, dictionary_array, var_count, gurobi_mode);
}

int autofill(Board* board, LinkedList *moves) {
    Board *b_cpy = brdcpy(board);
    int i, j, v, count = 0;
    for (i = 0; i < get_size(board); i++) {
        for (j = 0; j < get_size(board); j++) {
            if (get(b_cpy, i, j) == 0) {
                if (moves != NULL)
                    v = get_single_value(b_cpy, i, j);
                else
                    v = get_single_value(board, i, j);

                if (v != 0) {
                    if (moves != NULL)
                        set_command(board, moves, i, j, v);
                    else
                        set_value(board, i, j, v);
                    count++;
                }
            }
        }
    }
    free_board(b_cpy);
    return count;
}
int get_single_value(Board* board,int r, int c){
    int* sols = get_all_sol_cell(board, r, c);
    int single_sol=0, i;

    for(i=0;i<get_size(board);i++){
        if(sols[i]==1){
            if(single_sol != 0){
                free(sols);
                return 0;
            }
            single_sol = i+1;
        }
    }
    free(sols);
    return single_sol;
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
void change_cells_to(Board *board, LinkedListCells *values_list) {
    int i, len = get_len_linked_list_cells(values_list);
    Cell* curr;
    move_curr_to_head(values_list);
    for(i=0; i < len; i++){
        curr=get_curr_cell(values_list);
        set_cell(board,curr);
        advance_curr_cell(values_list);
    }
    move_curr_to_head(values_list);
}


void set_command(Board *board, LinkedList *moves, int r, int c, int value) {
    Node* curr = get_curr(moves);
    LinkedListCells* curr_old_values = get_old_values_cells_list(curr);
    LinkedListCells* curr_new_values = get_new_values_cells_list(curr);
    int prev_value=get(board,r,c);
    if(value!= prev_value)
    {
        add_cell_after_curr(curr_old_values, get_cell_cpy(board, r, c));
        if(is_error(board,r,c))
            validate_cell(board, curr_old_values, curr_new_values, r, c, prev_value, -1);
        validate_cell(board, curr_old_values, curr_new_values, r, c, value, 1);
        set_value(board,r,c,value);
        add_cell_after_curr(curr_new_values, get_cell_cpy(board, r, c));
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
    if(set_first_cell(brd_cpy, stack, next_cell) == 0){ /*finds the first empty cell to start from and push it to the stack*/
        free_board(brd_cpy);
        free(next_cell);
        free_stack(stack);
        return 0;
    }
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
        if(get_num_empty(brd_cpy) == 0)
            sol_count++;
        
    }
    free(next_cell);
    free_board(brd_cpy);
    free_stack(stack);
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
    Board* brd_cpy = brdcpy(board);
    int solvable =generate_solution(brd_cpy, 0);
    free_board(brd_cpy);
    return solvable;
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
    Board* brd_cpy = brdcpy(board);
    int solved, size = get_size(brd_cpy);
    double *solution = (double *) calloc(size * size * size, sizeof(double)), *sols;
    solved = solve(brd_cpy, solution, LP);
    if(solved){
        sols = get_probability_array(brd_cpy,solution,row,col);
        free(solution);
        return sols;
    }
    free(solution);
    return NULL;
}

int generate_command(Board *board, LinkedList *moves, int x, int y) {
    Board *brd_cpy;
    int i=0;

    while (i<1000){
        brd_cpy = brdcpy(board);
        if(generate_board(brd_cpy,x,y) == 1){
            copy_to_board(board, moves, brd_cpy);
            free_board(brd_cpy);
            return 1;
        }
        else
          free_board(brd_cpy);  
        i++;        
    }
    return 0;
}

/**
* @param board != NULL. THE METHOD CHANGES THE BOARD - MAKE SURE TO INPUT COPY OF THE ORIGINAL!
* @param x in range [0,get_num_empty(board)]
* @param y in range [0,(get_size(board))^2]
* @return 1 if succeeded to generate a solvable board. otherwise 0.
*/
int generate_board(Board* board, int x, int y) {
    if(fill_x_cells(board,x) == 0)
        return 0;
    if(!generate_solution(board,1))
        return 0;
    clear_cells(board, y);
    return 1;
}


/**
* @param board != NULL. THE METHOD CHANGES THE BOARD - MAKE SURE TO INPUT COPY OF THE ORIGINAL!
* @param x in range [0,get_num_empty(board)]
* @return 1 if succeeded.
* 0 otherwise - if got to a state such that a randomly chosen cell has no legal value to input.
*/
int fill_x_cells(Board* board, int x){
    int size = get_size(board);
    int r,c, empty=0;

    while(x>0){
        r = rand() % size;
        c = rand() % size;
        do {
            if (get(board, r, c) != 0) {
                r = rand() % size;
                if (get(board, r, c) != 0)
                    c = rand() % size;
                else
                    empty = 1;
            } else
                empty = 1;
        } while (!empty);
        if(set_random_value(board,r,c)){
            x--;
        }
        else
            return 0;
    }
    return 1;
}

/**
* sets random legal value in board(row,col). if there isn't one - returns 0.
* @param board != NULL. THE METHOD CHANGES THE BOARD - MAKE SURE TO INPUT COPY OF THE ORIGINAL!
* @param row in range [0,get_size(board)]
* @param col in range [0,get_size(board)]
* @return 1 if succeeded. 0 if there is no legal value for that cell in input board.
*/
int set_random_value(Board* board, int row, int col) {
    int *sol_arr = get_all_sol_cell(board, row, col);
    int num_sol = 0, i, sol_place;

    for (i = 0; i < get_size(board); i++) {
        if (sol_arr[i]==1)
            num_sol++;
    }
    if (num_sol == 0){
        free(sol_arr);
        return 0;
    }

    sol_place = rand() % num_sol;
    i=0;
    while(sol_place>=0){
        if(sol_arr[i]==1)
            sol_place--;
        i++;
    }
    set_value(board, row, col, i);
    free(sol_arr);
    return 1;
}

void clear_cells(Board* board, int num_keep){
    int i,j , size = get_size(board);
    fix_y_cells(board,num_keep);
    for(i=0;i<size;i++)
        for(j=0;j<size;j++)
            if(!is_fixed(board,i,j)){
                set_value(board,i,j,0);
            }
    unfix_all(board);

}
void fix_y_cells(Board *board, int num_fix){
    int size = get_size(board);
    int r,c, fixed=1;

    while(num_fix>0) {
        r = rand() % size;
        c = rand() % size;
        while (fixed) {
            if (is_fixed(board, r, c)){
                r = rand() % size;
                if (is_fixed(board, r, c))
                    c = rand() % size;
                else
                    fixed = 0;
            } else
                fixed = 0;
        }
        fix_cell(board, r, c);
        num_fix--;
        fixed = 1;
    }
}
void unfix_all(Board *board){
    int i,j, size = get_size(board);
    for(i=0; i<size;i++)
        for(j=0;j<size;j++)
            unfix_cell(board,i,j);
}

/**
* copies cells from second board to the first one. For every cell that has been changed, the previous
* values will be saved into the changed list of current move.
* @param to != NULL
* @param moves != NULL. the head of the list is the current move.
* @param from != NULL
*/
void copy_to_board(Board *to, LinkedList *moves, Board *from) {
    int i, j, size = get_size(to);
    Cell *cell_cpy_to, *cell_cpy_from;
    Node *curr = get_curr(moves);
    LinkedListCells *old_values = get_old_values_cells_list(curr);
    LinkedListCells *new_values = get_new_values_cells_list(curr);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            cell_cpy_from = get_cell_cpy(from, i, j);
            cell_cpy_to = get_cell_cpy(to, i, j);
            if (!is_equal_cell(cell_cpy_from, cell_cpy_to)) {
                add_cell_after_curr(old_values, cell_cpy_to);
                add_cell_after_curr(new_values, cell_cpy_from);
                set_cell(to, cell_cpy_from);
            } else{
                free(cell_cpy_to);
                free(cell_cpy_from);
            }
        }
    }
}

int guess_command(Board *board, LinkedList *moves, float threshold) {
    Board *brd_cpy = brdcpy(board);
    int solved, size = get_size(brd_cpy), i, j, rand_sol, last, len_prob_arr = 100;
    double *solution = (double *) calloc(size * size * size, sizeof(double)), *sols;
    int *prob_sols, *all_sols;

    if(solution == NULL){
        error("boardModifier","guess_command",1);
        exit(0);
    }
    solved = solve(brd_cpy, solution, LP);
    if (solved) {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if(get(brd_cpy,i,j) == 0){
                    prob_sols = (int *) calloc(len_prob_arr, sizeof(int));
                    if(prob_sols == NULL){
                        error("boardModifier","guess_command",1);
                        exit(0);
                    }
                    sols = get_probability_array(brd_cpy, solution, i, j);
                    all_sols = get_all_sol_cell(brd_cpy, i, j);
                    if(count_sols(all_sols,size) > 0){
                        fix_prob_arr(sols, all_sols, size); /*set 0 for illegal solutions*/
                        last = fill_score_arr(prob_sols, len_prob_arr, sols, size, threshold);
                        if(last > 0){
                            rand_sol = rand() % last;
                            set_value(brd_cpy, i, j, prob_sols[rand_sol]);
                        }
                    }
                    free(prob_sols);
                    free(all_sols);
                }
            }
        }
    } else{
        free(solution);
        free_board(brd_cpy);
        return 0;
    }
    copy_to_board(board, moves, brd_cpy);
    free(solution);
    free_board(brd_cpy);
    return 1;
}

/**
* sets value in len*score (floored) cells of arr, start at last.
* @param arr != NULL
* @param len = length of arr
* @param last = the last filled cell in arr
* @param value
* @param score
* @return numbers of non-empty cells in arr.
*/
int add_to_score_arr(int* arr, int len, int last, int value, double score) {
    int i;
    int normal_score = ((int)(len*score + 0.5));
    for (i = last; i < last + normal_score; i++) {
        arr[i] = value;
    }
    return i;
}

/**
* input array arr filled with solution values a cells in range [0,return value) such that
* the number of cells contain each solution are at the same proportion as their scores at input score array.
* @param arr != NULL. contains the probabilities of solution values for a cell
* @param len_arr length arr
* @param score != NULL.
* @param len_score length of score
* @param threshold
* @return the last index filled in score
*/
int fill_score_arr(int *arr, int len_arr, double *score, int len_score, float threshold) {
    int i, last = 0;
    for (i = 0; i < len_score; i++) {
        if(score[i]>=threshold)
            last = add_to_score_arr(arr, len_arr, last, i + 1, score[i]);
    }
    return last;
}

/**
* sets 0 in arr[i] if i+1 is illegal value by sol_arr.
* @param arr != NULL. contains the probabilities of solution values for a cell
* @param sol_arr != NULL. boolean array (for a cell) contains at index i whether i is a legal solutions of a cell
* @param size the length of input arrays
*/
void fix_prob_arr(double *arr, int *sol_arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (sol_arr[i] == 0)
            arr[i] = 0;
    }
}

Board *generate_basic_board() {
    Board* board = alloc_board(3,3);
    if(board==NULL){
        error("boardModifier","generate_basic_board",1);
        exit(0);
    }
    return board;
}
