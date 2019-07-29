/**

 Created by lenovo on ${DTE}
 **/

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"


char* modules[] = {"", "board","linkedList"};

void error(int error_code) {
    printf("Error: Module: ");
    int module = error_code / 10000;
    printf("module num = %d\n", module);
    printf("%s, ", modules[module]);
    int function = (error_code%1000)/100;
    if(function == 1){
        printf("memory allocation failure\n");
    }
    exit(0);

}
