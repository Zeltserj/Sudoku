
#include "fileaux.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
        fclose(f);
        return 0;
    }
    for(r=0;r<board->size;r++){
        for(c=0;c<board->size;c++){
            temp = fprintf(f,"%d",get(board,r,c));
            if(temp<0){
                error("fileaux","save",14);
                fclose(f);
                return 0;
            }
            if(is_fixed(board,r,c)){
                temp=fputs(".",f);
                if(temp<0){
                    error("fileaux","save",14);
                    fclose(f);
                    return 0;
                }
            }

            if(c<board->size-1)
                temp = fputs(" ",f);
            else
                temp = fputs("\n",f);
            if(temp<0){
                error("fileaux","save",14);
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return 1;
}

Board *load(char *path) {
    FILE *f = fopen(path, "r");
    Board *b;
    char *c = calloc(3, sizeof(char));
    int row, col, m, n, len_c, value,scf_ret;

    if (fscanf(f, "%d", &m) == 0 || fscanf(f, "%d", &n) == 0) {
        error("fileaux", "load", 15);
        free_load(f, NULL, c);
        return NULL;
    }
    b = alloc_board(m, n);
    for (row = 0; row < b->size; row++) {
        for (col = 0; col < b->size; col++) {
            scf_ret = fscanf(f,"%s",c);
            if (scf_ret == 0) {
                error("fileaux", "load", 15);
                free_load(f, b, c);
                return NULL;
            }
            if (scf_ret == EOF) {
                input_error(16);
                free_load(f, b, c);
                return NULL;
            }
            len_c = strlen(c);

            if (c[len_c - 1] == '.') {
                fix_cell(b, row, col);
                c[len_c - 1] = '\0';
            }
            value = atoi(c);
            if(value==0 && !is_zero(c)){
                input_error(18);
                free_load(f, b, c);
                return NULL;
            }
            if (value > b->size || value < 0) {
                input_error(17);
                free_load(f, b, c);
                return NULL;
            }
            set_value(b, row, col, value);
        }
    }
    scf_ret = fscanf(f,"%s",c);
    if(scf_ret > 0){
        input_error(19);
        free_load(f, b, c);
        return NULL;
    }
    free_load(f, NULL, c);
    return b;
}

void free_load(FILE *f, Board *b, char *c) {
    free(c);
    fclose(f);
    if(b!=NULL)
        free_board(b);
}
int is_zero(char* c){
    int i=0;
    while(i<strlen(c) && c[i]!= '\0'){
        if(c[i] != '0')
            return 0;
        i++;
    }
    return 1;
}



