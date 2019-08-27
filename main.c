#include <stdio.h>

#include "fileaux.h"
#include "boardModifier.h"

int main() {
	int i;
    Board *board = load("/specific/a/home/cc/students/csguests/jonathanz1/project/data4.txt"); /*ENTER CORRECT PATH*/
    double* solution = calloc(get_size(board)*get_size(board)*get_size(board),sizeof(double));
  
	solve(board,solution,0);
	double* prob_array = get_probability_array(board,solution,8,8);
	for(i = 0; i < get_size(board); i++){
		printf("prob[%d]: %f\n", i, prob_array[i]);
	}
	
}
