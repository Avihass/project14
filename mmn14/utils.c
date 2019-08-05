
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int haveError = 0;

void printError(char* ErrorMessage) {
    
    printf("Error! ");
    printf("%s\n", ErrorMessage);
    haveError = 1;
}

void printErrorInSrcFile(char* ErrorMessage) {
    
    printError(ErrorMessage);
    /* the line */
}

void printErrorAndStop(char* ErrorMessage) {
    
    printError(ErrorMessage);
    printf("the programme stop\n");
    exit(1);
}
