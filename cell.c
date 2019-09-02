
#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

void set_cell_value(Cell *cell, int value) { cell->value = value; }

void set_cell_fixed(Cell *cell, int fixed) { cell->fixed = fixed; }

void increase_cell_error(Cell *cell) { cell->error++; }

void decrease_cell_error(Cell *cell) { cell->error--; }
void set_cell_location(Cell *cell, int r, int c) {
    cell->row = r;
    cell->col = c;

}

int get_cell_value(Cell *cell) { return cell->value; }

int get_cell_fixed(Cell *cell) { return cell->fixed; }

int get_cell_error(Cell *cell) { return cell->error; }

int get_cell_row(Cell *cell) { return cell->row; }

int get_cell_col(Cell *cell) { return cell->col;}
int get_cell_was_changed(Cell *cell) { return cell->was_changed;}


Cell *alloc_cell(int r, int c) {
    Cell *cell = (Cell *) calloc(1, sizeof(Cell));
    if (cell == NULL) {
        error("board", "alloc_cell", 1); /*memory allocation failure*/
        exit(0);
    }
    set_cell_location(cell, r, c);
    return cell;
}

void cell_copy(Cell *to, Cell *from) {
    to->col = from->col;
    to->row = from->row;
    to->error = from->error;
    to->fixed = from->fixed;
    to->value = from->value;
}

void set_cell_error(Cell *cell, int error_value) { cell->error = error_value; }

int is_equal_cell(Cell *c1, Cell *c2) {
    if (c1->value != c2->value || c1->error != c2->error || c1->fixed == c2->fixed || c1->col != c2->col ||
        c1->row != c2->row)
        return 0;
    return 1;
}

void set_cell_was_changed(Cell *cell, int was_changed) {
    cell->was_changed = was_changed;
}

