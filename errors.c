
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"


char* errors(int errornum){
    char* out = NULL;
    switch(errornum){
        case 1:
            out = "memory failure";
            break;
        case 2:
            out = "empty list";
            break;
        case 3:
            out = "next cell is null";
            break;
        case 4:
            out = "previous cell is null";
            break;
        case 5:
            out = "null name";
            break;
        case 6:
            out = "INVALID command name";
            break;

        default:
            out = "Unknown error";


    }

    return out;


}

void error(char *module, char *function, int error_code) {
   printf("Error: in module: %s, function %s : %s", module, function, errors(error_code));
}
