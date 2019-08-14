
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "assembly.h"

int haveError = 0;
int haveErrorInLine = 0;

void printError(char* ErrorMessage) {
    
    printf("Error! ");
    printf("%s\n", ErrorMessage);
    haveError = 1;
}

void printErrorInSrcFile(char* ErrorMessage) {
    
    printError(ErrorMessage);
    haveErrorInLine = 1;
    printf("(at the line: %d)\n\n", actLineInSrc);
}

void printErrorAndStop(char* ErrorMessage) {
    
    printError(ErrorMessage);
    printf("the programme stop\n");
    exit(1);
}
