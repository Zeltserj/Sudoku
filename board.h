
#include "errors.h"

#ifndef PROJECT_BOARD_H
#define PROJECT_BOARD_H
/**
 * cell structure for use in the autofill list, and board structure contains a matrix of these cells
 * contains all information regarding cells (value, erroneous in relation to the board, fixed/free, and cell's location
 */
 extern int mode;
 extern int mark_errors;
typedef struct GameCell{
    int value;
    int fixed;
    int error;
    int row;
    int col;
} Cell;

/**
 * Board stucture containg a 2d matrix of *Cell. also keeps it's parameters and number of empty cells
 *
 */
typedef struct GameBoard{
    int rows;
    int cols;
    int size;
    Cell*** matrix;
    int num_empty;
}Board;

/**
 *
 * @return allocates a new cell or null on memory fault
 */
Cell *alloc_cell(int r, int c);
/**
 *
 * @param r
 * @param c
 * @param value
 * @return allocates a new board of size r*c*c*r or fails
 */
void set_cell_value(Cell *cell, int value);
void set_cell_fixed(Cell *cell, int fixed);
void set_cell_error(Cell *cell, int is_error);
int get_cell_value(Cell *cell);
int get_cell_fixed(Cell *cell);
int get_cell_error(Cell *cell);
int get_cell_row(Cell *cell);
int get_cell_col(Cell *cell);

Cell *** alloc_matrix(int size);
Board *alloc_board(int r, int c);
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
#endif //PROJECT_BOARD_H
