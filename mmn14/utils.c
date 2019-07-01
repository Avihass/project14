
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void printError(char* ErrorMessage) {
    
    printf("Error! ");
    printf("%s\n", ErrorMessage);
    printf("the programme stop\n");
    exit(0);
}
