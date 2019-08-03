
#ifndef SPROJECT_FILEAUX_H
#define SPROJECT_FILEAUX_H

#include "board.h"
#include <stdio.h>

Board* load(char* path);
int is_zero(char* c);
void free_load(FILE *f, Board *b, char *c);
/**
* @param board != NULL
* @param path - the new file will be saved to path location
* @return 1 if succeeded.
* otherwise - file opening error / file writing error - prints the error and returns 0.
*/
int save(Board *board, char *path);


#endif /*SPROJECT_FILEAUX_H*/
