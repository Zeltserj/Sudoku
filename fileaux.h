
#ifndef SPROJECT_FILEAUX_H
#define SPROJECT_FILEAUX_H

#include <stdio.h>
#include "board.h"


/**
*
* @param path - the new file will be saved to path location.
* @param edit_or_solve - 0 for edit, 1 for solve
* @return if succeeded returns board with the values loaded.
* otherwise, returns NULL and prints the proper error.
*/
Board *load(char *path, int edit_or_solve);

/**
* @param board != NULL
* @param path - the new file will be saved to path location
* @return 1 if succeeded.
* otherwise - file opening error / file writing error - prints the error and returns 0.
*/
int save(Board *board, char *path);


#endif /*SPROJECT_FILEAUX_H*/
