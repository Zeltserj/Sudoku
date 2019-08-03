
#include "fileaux.h"
#include <stdio.h>

int save(Board *board, char *path) {
    FILE* f = fopen(path,"w");
    int temp;

    if(f==NULL){
        error("fileaux","save",13);
        return 0;
    }
    int r,c;
    temp = fprintf(f,"%d %d\n",board->rows_block,board->cols_block);
    if(temp<0){
        error("fileaux","save",14);
        return 0;
    }
    for(r=0;r<board->size;r++){
        for(c=0;c<board->size;c++){
            temp = fprintf(f,"%d",get(board,r,c));
            if(temp<0){
                error("fileaux","save",14);
                return 0;
            }
            if(is_fixed(board,r,c)){
                temp=fputs(".",f);
                if(temp<0){
                    error("fileaux","save",14);
                    return 0;
                }
            }

            if(c<board->size-1)
                temp = fputs(" ",f);
            else
                temp = fputs("\n",f);
            if(temp<0){
                error("fileaux","save",14);
                return 0;
            }
        }
    }
    fclose(f);
    return 1;
}
