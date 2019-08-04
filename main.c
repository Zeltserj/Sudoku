#include <stdio.h>
#include "board.h"
#include "fileaux.h"

int main() {
    /*FILE* f = fopen("C:\\Users\\Or Nechemia\\CLionProjects\\SProject\\testopening","w");*/
    /*Board* b = alloc_board(1,2);*/
    Board* b2;
    /*set(b,0,1,2);
    set(b,0,0,1);
    set(b,1,1,1);
    fix_cell(b,0,0);
    fix_cell(b,1,1);
    print_board(b);
    save(b,"C:\\Users\\Or Nechemia\\CLionProjects\\SProject\\test_faux.txt");
*/

    b2=load("C:\\Users\\Or Nechemia\\CLionProjects\\SProject\\test_faux.txt");
    if(b2!= NULL)
        print_board(b2);


    /*fclose(f);*/
}
