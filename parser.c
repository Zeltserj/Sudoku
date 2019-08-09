
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "errors.h"

int get_whitespace_offset(char *copy);

void parse_filepath(char *filepath);
char *cpy_input(char *str, int from, int to);
/**
 *
 * @param string given from user after the command name
 * @return an array of integers corresponding to the parameters. if index i of parameters failed (not an integer), -1 is returned
 */
int parse_input_parameters(char *string, int *parameters) {
    int i = 0, len;
    char *ptr, *input_copy, *end_ptr;
    char delim[] = " \t";
    if (string == NULL) {
        return 0;
    }
    len = (int) strlen(string);
    input_copy = (char *) malloc(len * sizeof(char));
    if (input_copy == NULL) {
        error("parser", "parse_input_parameters", 1);
        exit(0);
    }
    strcpy(input_copy, string);
    ptr = strtok(input_copy, delim);
    while (ptr != NULL && ptr[0] != '\n') {
        parameters[i] = (int) strtol(ptr, &end_ptr, 10);
        if (ptr == end_ptr) {
            parameters[i] = -1;
        }
        i++;
        ptr = strtok(NULL, delim);
    }
    if (i > 0) {
        parameters = realloc(parameters, (i) * sizeof(int));
        if (parameters == NULL) {
            error("parser", "parse_input_parameters", 1);
            exit(0);
        }
    }
    free(input_copy);

    return i;
}

Command *parse_input(char *input) {
    Command* out = calloc(1, sizeof(Command));
    const char *delim = " \t"; /*unlike HW3, we don't take \n as input*/
    int *parameters = calloc(256, sizeof(int));
    int i, len = (int)strlen(input), offset, num_parameters;
    command_type type;
    char *input_copy = (char*)malloc(len* sizeof(char)), *name = NULL, *ptr = NULL, *temp;
    float threshold;
    if(out == NULL ||input_copy == NULL || parameters == NULL){
        error("parser", "parser_input", 1);
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

    ptr = cpy_input(input,i+offset,len-1);

    type = (int)get_type(out);
    if(type == EDIT){ /*edit command has optional parameter which will be null if not given */
        ptr = strtok(NULL, delim);
        temp = cpy_input(ptr,0,strlen(ptr));
        set_filepath(out, temp);
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
        /*printf("parse_input: before ptr\n");
        free (ptr);
        printf("parse_input: after ptr\n");*/
        ptr = strtok(NULL, delim);
        temp = cpy_input(ptr,0,strlen(ptr));
        set_filepath(out, temp);
        parse_filepath(get_filepath(out));
        if(ptr == NULL || ptr[0] == '\n'){
            set_num_parameters(out, 0);
            return out;
        }
        else{
            set_num_parameters(out, 1);
        }
        ptr = strtok(NULL, delim);
        if(ptr != NULL && ptr[0] != '\n'){
            set_num_parameters(out, 2);
            /*to or more parameters doesn't change*/
        }
        return out;
    }
    num_parameters = parse_input_parameters(ptr, parameters);
    set_num_parameters(out, num_parameters);
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

/**
* @param from in range of str length
* @param to in range of str length
* @return copy of the substring [from,to] in str
*/
char *cpy_input(char *str, int from, int to) {
    int size = to - from + 1, i, j = 0;
    char *new_str = calloc(size, sizeof(char));

    for (i = from; i <= to; i++) {
        new_str[j] = str[i];
        j++;
    }
    return new_str;
}
