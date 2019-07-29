/**

 Created by lenovo on ${DTE}
 **/
#include "errors.h"

#ifndef PROJECT_BOARD_H
#define PROJECT_BOARD_H
typedef struct GameCell{
    int value;
    int fixed;
    int error;
    int row;
    int col;
} Cell;
typedef struct GameBoard{
    int rows;
    int cols;
    int size;
    Cell** matrix;
    int num_empty;
}Board;

/**
 *
 * @return allocates a new cell
 */
Cell* alloc_cell();
/**
 *
 * @param r
 * @param c
 * @param value
 * @return allocates a new board of size r*c*c*r or fails
 */

Cell** alloc_matrix(int size);
Board* alloc_board(int r, int c, int value);
/**
 *
 * @param board != null
 * @param r < board.rows
 * @param c < board.cols
 * @param value
 *
 * sets cell at (r, c) to value
 */
void set(Board* board, int r, int c, int value);
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
int free_cell(Cell* cell);

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
int free_board(Board* board);
/**
 *
 * @param cell
 * @return frees cell from heap
 */
int free_cell(Cell* cell);
/**
 *
 * @param matrix
 * @return frees matrix of cells from heap
 */
int free_matrix(Cell** matrix);
/**
 *
 * @param board != null
 * @return new copy of board
 */
int brdcpy(Board* board);


#endif //PROJECT_BOARD_H
