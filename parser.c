
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
    input_copy = (char *) malloc((len + 1) * sizeof(char));
    if (input_copy == NULL) {
        error("parser", "parse_input_parameters", 1);
        exit(0);
    }
    strcpy(input_copy, string);
    ptr = strtok(input_copy, delim);
    while (ptr != NULL && ptr[0] != '\n' && ptr[0] != '\r') {
        parameters[i] = (int) strtol(ptr, &end_ptr, 10);
        if (ptr == end_ptr) {
            parameters[i] = -1;
        }
        i++;
        ptr = strtok(NULL, delim);
    }
    if (i > 0) {
        if (parameters == NULL) {
            error("parser", "parse_input_parameters", 1);
            exit(0);
        }
    }
    free(input_copy);

    return i;
}

void parse_input(char *input, Command *out) {
    const char *delim = " \t"; /*unlike HW3, we don't take \n as input*/
    int *parameters = calloc(256, sizeof(int));
    int i, len = (int) strlen(input), offset, num_parameters,temp_yx;
    command_type type;
    char *input_copy = (char *) malloc((len + 1) * sizeof(char)), *name = NULL, *ptr = NULL, *temp = NULL, *token;
    float threshold;
    char c;

    if (input_copy == NULL || parameters == NULL) {
        error("parser", "parser_input", 1);
        exit(0);
    }
    if (len > 256) {
        c = getchar();
        while (c != '\n' && c != '\r') {
            c = getchar();
        }
        input_error(11);
        set_type(out, "invalid");
        free(input_copy);
        free(parameters);
        return;
    }
    strcpy(input_copy, input);
    name = strtok(input_copy, delim);
    set_type(out, name);
    type = get_type(out);
    if (type == INVALID) {
        input_error(6);
        free(input_copy);
        free(parameters);
        return;
    }
    i = (int) strlen(name);
    offset = get_whitespace_offset(input_copy);

    ptr = cpy_input(input, i + offset, len - 1);
    type = get_type(out);
    if (type == EDIT) { /*edit command has optional parameter which will be null if not given */
        temp = strtok(ptr, delim);
        if (temp != NULL) {
            temp = cpy_input(ptr, 0, strlen(ptr));
            set_filepath(out, temp);
            parse_filepath(get_filepath(out));
        }
        if (temp == NULL || temp[0] == '\n'|| temp[0] == '\r') {
            set_num_parameters(out, 0);
        } else {
            temp = strtok(NULL, delim);
            if (temp == NULL || temp[0] == '\n') {
                set_num_parameters(out, 1);
            } else {
                set_num_parameters(out, 2);
            }
        }


    } else if (type == GUESS) {
        token = strtok(ptr, delim);
        if (token == NULL) {
            set_num_parameters(out, 0);
        } else {
            threshold = (float) strtof(ptr, &ptr);
            set_threshold(out, threshold);
            set_num_parameters(out, 1);
            token = strtok(NULL, delim);
            if (token != NULL && (token[0] != '\n' || token[0] != '\r')) {
                set_num_parameters(out, 2);
            }
        }
    } else if (type == SOLVE || type == SAVE) /*all commands which don't need int parameters*/{
        token = strtok(ptr, delim);
        temp = cpy_input(token, 0, strlen(token));
        set_filepath(out, temp);
        parse_filepath(get_filepath(out));
        if (token == NULL || token[0] == '\n' || token[0] == '\r') {
            set_num_parameters(out, 0);
        }
        else{
            set_num_parameters(out, 1);
        }
        token = strtok(NULL, delim);
        if (token != NULL && (token[0] != '\n' || token[0] != '\r')) {
            set_num_parameters(out, 2);
            /*to or more parameters doesn't change*/
        }
    } else {
        num_parameters = parse_input_parameters(ptr, parameters);
        if(type == SET || type == HINT || type == GUESS_HINT){
            temp_yx = parameters[0];
            parameters[0] = parameters[1];
            parameters[1] = temp_yx;
        }
        set_num_parameters(out, num_parameters);
        set_parameter(out, parameters, 0);
    }
    free(input_copy);
    free(ptr);
    free(parameters);
    return;
}


/**
 *
 * @param filepath
 * removes the last '\n' leftover character if one exists
 */
void parse_filepath(char *filepath) {
    if (filepath == NULL) {
        return;
    }

    int len = (int) strlen(filepath);
    if (filepath[len - 1] == '\n') {
        filepath[len - 1] = '\0';
    }
    if (filepath[len - 2] == '\r') {
        filepath[len - 2] = '\0';

    }
}

/**
 *
 * @param copy != NULL
 * @return number of chars until first non white space
 */
int get_whitespace_offset(char *copy) {
    int i = 0;
    while (isspace(copy[i])) {
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
    char *new_str = calloc(size+1, sizeof(char));

    for (i = from; i <= to; i++) {
        new_str[j] = str[i];
        j++;
    }
    return new_str;
}
