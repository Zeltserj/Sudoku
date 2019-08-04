

#ifndef PROJECT_BOARD_H
#define PROJECT_BOARD_H
#include "errors.h"
#include "cell.h"

/**
 * Board structure containing a 2d matrix of *Cell. also keeps its parameters and number of empty cells
 *
 */
typedef struct GameBoard{
    int rows_block;
    int cols_block;
    int size;
    Cell*** matrix;
    int num_empty;
}Board;
/**
 * All functions assume no null is given, as such is caught beforehand
 */


/**
* copies the values of cell to board[cell.row][cell.col]
* @param board != NULL
* @param cell != NULL
*/
void set_cell(Board *board, Cell *cell);

Cell *** alloc_matrix(int size);
Board *alloc_board(int r, int c);

/*TODO: Or: board.size???*/
/**
 *
 * @param board != null
 * @param r < board.rows
 * @param c < board.cols
 * @param value
 *
 * sets cell at (r, c) to value
 */


void set(Board* board, int r, int c, int value); /*TODO: check whether r and c are in range*/
/**
 *
 * @param board != null
 * @param r < board.rows
 * @param c < board.cols
 * @return value at cell i,j 0 if empty
 */
int get(Board* board, int r, int c);

/**
 *
 * @param cell
 * @return 1 if successful or error code
 */


/**
 *
 * @param board != null
 * @param r < board.rows
 * @param c < board.cols
 * @return 1 iff cell fixed
 */
int is_fixed(Board* board, int r, int c);
/**
 *
 * @param board != null
 * @param r < board.rows
 * @param c < board.cols
 * @return 1 iff cell is erroneous
 */
int is_error(Board* board, int r, int c);
/**
 *
 * @param board
 * @return frees board struct from heap
 */

void free_matrix(Cell*** matrix, int size);
void free_board(Board* board);

/**
 *
 * @param board != null
 * @return new copy of board
 */
Board * brdcpy(Board* board);

/**
 *
 * @param board
 * @param r < board.rows
 * @param c < board.cols
 *
 * sets fixed/erroneous cells in the board
 */
void fix_cell(Board* board, int r, int c);
void unfix_cell(Board* board, int r, int c);
void set_erroneous(Board* board, int r, int c);
void set_legal(Board* board, int r, int c);
/**
 *
 * @param board != NULL
 *
 * prints board to user
 */
void print_board(Board *board);


/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @param mark_illegal - if 1, marks same row cells as erroneous if has the same value (the input value)
* otherwise, doesn't
* @return 1 if value does not exist in row r on board (exclude cell board[r][c]). 0 otherwise.
*/
int is_legal_row(Board *board, int r, int c, int value, int mark_illegal);
/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @param mark_illegal - if 1, marks same col cells as erroneous if has the same value (the input value)
* otherwise, doesn't
* @return 1 if value does not exist in column c on board (exclude cell board[r][c]). 0 otherwise.
*/
int is_legal_col(Board *board, int r, int c, int value, int mark_illegal);
/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @param mark_illegal - if 1, marks same block cells as erroneous if has the same value (the input value)
* otherwise, doesn't
* @return 1 if value does not exist in cell's (board[r][c]) block on board (exclude that cell).
* 0 otherwise.
*/
int is_legal_block(Board *board, int r, int c, int value, int mark_illegal);
/**
* @param board != NULL
* @param r in range [0,board.size]
* @param c in range [0,board.size]
* @param value in range [0,board.size]
* @return 1 if value is legal for the cell in board[r][c]. otherwise 0.
*/
int is_legal_value(Board *board, int r, int c, int value);

/*int is_legal_value_set(Board *board, int r, int c, int value, int mark_illegal);*/

#endif
