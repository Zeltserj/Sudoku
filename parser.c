/**

 Created by lenovo on ${DTE}
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "errors.h"




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
    i = (int)strlen(name);
    ptr = &input_copy[i] + i;
    set_type(out, name);
    type = (int)get_type(out);
    if(type == 2){ /*edit command has optional parameter which will be null if not given */
        ptr = strtok(NULL, delim);
        set_filepath(out, ptr);
        set_num_paramters(out, 0);
        return out;
    }
    else if(type == 1 || type == 11) /*all commands which don't need int parameters*/{
        ptr = strtok(NULL, delim);
        set_filepath(out, ptr);
        ptr = strtok(NULL, delim);
        if(ptr != NULL){
            set_num_paramters(out, 2); /*to or more parameters doesn't change*/
        }
        else{
            set_num_paramters(out, 1);
        }
        return out;
    }
    parameters = parse_input_parameters(ptr, out);
    set_num_paramters(out, parameters[0]);
    parameters = &parameters[0] + 1;
    set_parameter(out, parameters);
    return out;
}

/**
 *
 * @param string given from user after the command name
 * @return an array of integers corresponding to the parameters. if index i of parameters failed (not an integer), -1 is returned
 */
int *parse_input_parameters(char *string, Command *command) {
    int i = 1, len ;
    int* out;
    char *ptr, *input_copy, *end_ptr;
    char delim[] = " \t";
    if(string == NULL){
        return NULL;
    }
    len = (int)strlen(string);
    out = calloc(len, sizeof(int));
    input_copy = (char*)malloc(len* sizeof(char));
    if(out == NULL || input_copy == NULL){
        error("parser", "parse_input_parameters", 1);
        exit(0);
    }
    strcpy(input_copy, string);
    ptr = strtok(input_copy, delim);
    while(ptr != NULL){
       out[i] =(int) strtol(ptr, &end_ptr, 10);
       if(ptr == end_ptr){
           set_type(command, "invalid");
           input_error(10);
       }
       i++;
       ptr = strtok(NULL, delim);
    }
    out = realloc(out,  (i+1)*sizeof(int));
    out[0] = i;
    return out;
}
