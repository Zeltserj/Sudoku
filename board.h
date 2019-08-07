

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

/**
 *
 * @param board != null
 * @param r < board.size
 * @param c < board.size
 * @param value
 *
 * sets cell at (r, c) to value
 */
void set_value(Board *board, int r, int c, int value); /*TODO: check whether r and c are in range*/
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
 * @param board != NULL
 * @return number of empty cells in the board, by attribute
 */
int get_num_empty(Board* board);

int get_size(Board* board);
int get_block_rows(Board* board);
int get_block_cols(Board* board);


Cell* get_cell_cpy(Board* board, int r, int c);

/**
 * @param board != null
 * @param r < board.size
 * @param c < board.size
 * @return 1 iff cell fixed
 */
int is_fixed(Board* board, int r, int c);
/**
 *
 * @param board != null
 * @param r < board.size
 * @param c < board.size
 * @return 1 iff cell is erroneous
 */
int is_error(Board* board, int r, int c);
/**
 *
 * @param board != NULL
 * @return 1 iff board as an erroneous cell (cell with error attribute == 1) 0 otherwise
 */
int is_erroneous(Board* board);

/**
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
 * @param r < board.size
 * @param c < board.size
 * sets fixed/erroneous cells in the board
 */
void fix_cell(Board* board, int r, int c);
void unfix_cell(Board* board, int r, int c);
/*void set_erroneous(Board* board, int r, int c);
* void set_legal(Board* board, int r, int c);*/
int increase_error(Board* board, int r, int c);
int decrease_error(Board* board, int r, int c);


/**
 * @param board != NULL
 * prints board to user
 */
void print_board(Board *board);

#endif
