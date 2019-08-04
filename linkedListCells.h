
#ifndef SP_LINKEDLISTCELLS_H
#define SP_LINKEDLISTCELLS_H

#include <stdlib.h>
#include "cell.h"
#include "errors.h"

typedef struct NodeLinkedListCells{
    Cell* c;
    struct NodeLinkedListCells* prev;
    struct NodeLinkedListCells* next;
}NodeCell;

typedef struct LinkedListCells{
    NodeCell* head;
    NodeCell* current;
    int len;
}LinkedListCells;

/**
* adds (cfter current) a node to the list contains the cell
* @param list of nodes type NodeCell
* @param c a cell to add
*/
void add_cell_after_curr(LinkedListCells *list, Cell *c);
/**
* @param list
* @return cell of current node of list. NULL if list is empty
*/
Cell *get_curr_cell(LinkedListCells *list);
/**
*
* @param list
* @return cell of head node of list. NULL if list is empty
*/
Cell * get_head_cell(LinkedListCells *list);

int get_len(LinkedListCells* list);
/**
* moves current node to next cell on the list
* @return 1 if succeeded, 0 if there isn't next cell
*/
int next_cell(LinkedListCells *list);
/**
* moves current node to previous cell on the list
* @return 1 if succeeded, 0 if there isn't previous cell
*/
int prev_cell(LinkedListCells *list);

/**
* moves the current cell pointer of the input list to the head cell.
* @param list != NULL
*/
void move_curr_to_head(LinkedListCells *list);

/**
* move current pointer to the next cell of the list
* @param list != NULL
* @return 1 if succeeded, 0 if there is'nt next cell after current
*/
int advance_curr_cell(LinkedListCells *list);

/**
* create new node contains cell c. next and prev are NULL
* @param c - cell the output node contains
* @return the new node
*/
NodeCell * alloc_node_cell(Cell* c);

/**
* allocate memory for linked list of cells.
* @return the empty list
*/
LinkedListCells* alloc_linked_list_cells();

/**
* free the fields of input node
* @param node != NULL
*/
void free_node_cell(NodeCell *node);

/**
* free all the nodes in input list and its other fields
* @param list != NULL
*/
void free_linked_list_cells(LinkedListCells *list);
#endif
