/**

 Created by lenovo on ${DTE}
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "errors.h"

int get_whitespace_offset(char *copy);

void parse_filepath(char *filepath);

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
    while(ptr != NULL && ptr[0] != '\n'){
        out[i] =(int) strtol(ptr, &end_ptr, 10) ;
        if(ptr == end_ptr){
            out[i] = -1;
        }
        i++;
        ptr = strtok(NULL, delim);
    }
    out = realloc(out,  (i)*sizeof(int));
    if(out == NULL){
        error("parser", "parse_input_parameters", 1);
        exit(0);
    }
    out[0] = i-1;
    free(input_copy);
    return out;
}

Command *parse_input(char *input) {
    Command* out = calloc(1, sizeof(Command));
    const char *delim = " \t"; /*unlike HW3, we don't take \n as input*/
    int *parameters = NULL;
    int i, len = (int)strlen(input), offset;
    command_type type;
    char *input_copy = (char*)malloc(len* sizeof(char)), *name = NULL, *ptr = NULL, *end_ptr = NULL;
    FILE *fp = fopen("C:\\Users\\lenovo\\CLionProjects\\SP\\file.txt", "w");
    float threshold;
    if(out == NULL ||input_copy == NULL ){
        error("parser", "paser_input", 1);
        exit(0);
    }
    if(len > 256){
        input_error(11);
        set_type(out, "invalid");
        return out;
    }
    strcpy(input_copy, input);
    name = strtok(input_copy, delim);
    set_type(out, name);
    type = get_type(out);
    if(type == INVALID){
        input_error(6);
        return out;
    }
    i = (int)strlen(name);
    offset = get_whitespace_offset(input_copy);
    ptr = &input[offset+i];
    type = (int)get_type(out);
    if(type == EDIT){ /*edit command has optional parameter which will be null if not given */
        ptr = strtok(NULL, delim);
        set_filepath(out, ptr);
        parse_filepath(get_filepath(out));
        if(ptr == NULL || ptr[0] == '\n'){
            set_num_parameters(out, 0);
        }
        else{
            ptr = strtok(NULL, delim);
            if(ptr == NULL || ptr[0] == '\n'){
                set_num_parameters(out, 1);
            }
            else{
                set_num_parameters(out, 2);
            }
        }
        return out;
    }
    else if(type == GUESS){
        ptr = strtok(NULL, delim);
        if(ptr == NULL){
            set_num_parameters(out, 0);
        }
        else{
            threshold = (float)strtof(ptr, &ptr);
            set_threshold(out, threshold);
            set_num_parameters(out, 1);
            ptr = strtok(NULL, delim);
            if(ptr != NULL && ptr[0] != '\n'){
                set_num_parameters(out, 2);
            }
        }
        return out;
    }
    else if(type == SOLVE || type == SAVE) /*all commands which don't need int parameters*/{
        ptr = strtok(NULL, delim);
        set_filepath(out, ptr);
        parse_filepath(get_filepath(out));
        if(ptr == NULL || ptr[0] == '\n'){
            set_num_parameters(out, 0);
            return out;
        }
        else{
            set_num_parameters(out, 1);
        }
        ptr = strtok(NULL, delim);
        fputs(ptr, fp);
        fclose(fp);
        if(ptr != NULL && ptr[0] != '\n'){
            set_num_parameters(out, 2);
            /*to or more parameters doesn't change*/
        }


        return out;
    }
    parameters = parse_input_parameters(ptr, out);
    set_num_parameters(out, parameters[0]);
    parameters = &parameters[0] + 1;
    set_parameter(out, parameters);
    return out;
}


/**
 *
 * @param filepath
 * removes the last '\n' leftover character if one exists
 */
void parse_filepath(char *filepath) {
    if(filepath == NULL){
        return;
    }
    int len = (int)strlen(filepath);
    if(filepath[len-1] == '\n'){
        filepath[len-1] = '\0';
    }

}

/**
 *
 * @param copy != NULL
 * @return number of chars until first non white space
 */
int get_whitespace_offset(char *copy) {
    int i = 0;
    while(isspace(copy[i])){
        i++;
    }
    return i;
}
