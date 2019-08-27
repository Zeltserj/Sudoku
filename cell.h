
#ifndef SPROJECT_CELL_H
#define SPROJECT_CELL_H

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

/**
 *
 * @param cell != NULL
 * @param fixed == 1 || fixed == 0
 * sets fixed value of cell to fixec
 *
 */
void set_cell_fixed(Cell *cell, int fixed);
/**
 *
 * @param cell != NULL
 * increments number of conflicts by 1 in cell
 */
void increase_cell_error(Cell *cell);
/**
 *
 * @param cell != NULL
 * increments number of conflicts by 1 in cell
 */
void decrease_cell_error(Cell *cell);

/**
 *
 * @param cell != NULL
 * @param error_value >= 0
 *
 * sets cell.error to value
 */
void set_cell_error(Cell *cell, int error_value);
/**
 *
 * @param cell != NULL
 * @param r in range[0,size)
 * @param c in range[0,size)
 */
void set_cell_location(Cell *cell, int r, int c);

/**
 *
 * @param cell != NULL
 * @return cell.value
 */
int get_cell_value(Cell *cell);
/**
 *
 * @param cell != NULL
 * @return cell.fixed
 */
int get_cell_fixed(Cell *cell);
/**
 *
 * @param cell != NULL
 * @return cell.error
 */
int get_cell_error(Cell *cell);
/**
 *
 * @param cell != NULL
 * @return cell.row
 */
int get_cell_row(Cell *cell);

/**
 *
 * @param cell != NULL
 * @return cell.col
 */
int get_cell_col(Cell *cell);

/**
 *
 * @param to != NULL, source cell
 * @param from != NULL, destination  cell
 *
 * upon return from function, for all attribute x in Cell structure, to.x == from.x
 */
void cell_copy(Cell *to, Cell *from);

/**
 *
 * @param c1
 * @param c2
 * @return 1 iff for all attribute x in Cell structure, to.x == from.x
 */
int is_equal_cell(Cell* c1, Cell* c2);
#endif /*SPROJECT_CELL_H*/
