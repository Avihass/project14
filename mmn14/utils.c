
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "assembly.h"
#include "readSource.h"

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

int instructAdCount(instructField instruction) {
    
    /* instruction take 1 adress in memory */
    int AdCount = 1;
    int haveRegInSrc = 0;
    
    /* if there is a source operand */
    if (instruction.srcOp.type != -1) {
        
        if (instruction.srcOp.type == imediate_met || instruction.srcOp.type == direct_met)
            AdCount++;
        
        else if (instruction.srcOp.type == index_met)
            AdCount += 2;
        
        /* source type is register_met */
        else {
            
            AdCount++;
            haveRegInSrc = 1;
        }
    }
    
    if (instruction.destOp.type != -1) {
        
        if (instruction.destOp.type == imediate_met || instruction.srcOp.type == direct_met)
            AdCount++;
        
        else if (instruction.destOp.type == index_met)
            AdCount += 2;
        
        else if (!haveRegInSrc)
            AdCount++;
    }
    
    return AdCount;
}
