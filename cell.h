
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
 * getters and setters to cell structure
 */
void set_cell_fixed(Cell *cell, int fixed);
void set_cell_error(Cell *cell, int is_error);
void set_cell_location(Cell *cell, int r, int c);
int get_cell_value(Cell *cell);
int get_cell_fixed(Cell *cell);
int get_cell_error(Cell *cell);
int get_cell_row(Cell *cell);
int get_cell_col(Cell *cell);

void cell_copy(Cell *to, Cell *from);

#endif /*SPROJECT_CELL_H*/
