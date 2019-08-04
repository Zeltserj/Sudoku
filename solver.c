
#include "solver.h"
#include <stdlib.h>

int *get_all_sol_cell(Board *board, int r, int c) {
    int* sol = calloc(get_size(board), sizeof(int));
    int i, num_sol=0;
    if(sol==NULL){
        error("solver","get_all_sol_cell",1);
        exit(0);
    }
    for(i=0;i<get_size(board);i++){
        if(is_legal_value(board, r, c, i + 1)) {
            sol[i] = 1;
            num_sol++;
        }
    }
    if(num_sol==0)
        return NULL;
    return sol;
}
