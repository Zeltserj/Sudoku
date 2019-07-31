/**

 Created by lenovo on ${DTE}
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "errors.h"


int *parse_input_parameters(char *string);

char *parse_filepath(char *string);

Command *parse_input(char *input) {
    Command* out = calloc(1, sizeof(Command));
    const char *delim = " \t"; /*unlike HW3, we don't take \n as input*/
    int *parameters = NULL;
    int i, type, len = (int)strlen(input);
    char *input_copy = (char*)malloc(len* sizeof(char)), *ptr = NULL, *filepath = NULL, *name = NULL;
    if(out == NULL ||input_copy == NULL ){
        error("parser", "paser_input", 1);
        exit(0);
    }
    strcpy(input_copy, input);
    name = strtok(input_copy, delim);
    set_type(out, name);
    type = (int)get_type(out);
    ptr = strtok(NULL, delim);
    if(type == 1 || type == 2 || type == 11) /*all commands which don't need int parameters*/{
        set_filepath(out, ptr);
        ptr = strtok(NULL, delim);
        set_parameter(out, parse_input_parameters(ptr));
        return out;
    }
    ptr = strtok(NULL, delim);
    if(ptr != NULL){
        parameters = parse_input_parameters(ptr);
        set_parameter(out, parameters);
    }

    return out;
}

int *parse_input_parameters(char *string) {
    int i, len = (int)strlen(string), sum = 0, j = 0;
    int* out = calloc(len, sizeof(int));

    if(string == NULL){
        return NULL;
    }
    if(out == NULL){
        error("parser", "parse_input_parameters", 1);
    }
    for(i = 0; i < len; i++){
        if(string[i] == '\t' || string[i] == ' '){
            j++;
        }

        else{
            out[j] = out[j]*10 + (string[i]-48);
        }

    }
    return out;
}
