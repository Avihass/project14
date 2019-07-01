
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "readSource.h"
#include "utils.h"

int main(int argc, const char * argv[]) {
    
    char srcFileName[MAX_FILE_NAME + 3];
    FILE *srcFile;
    int readedFirstWord;
    int endOfSrc = 0;
    
    if (strlen(argv[1]) >= MAX_FILE_NAME) {
        
        printf("Error! the source file name is to long\n");
        printf("the programme stop\n");
        exit(0);
    }
    
    strcpy(srcFileName, argv[1]);
    strcat(srcFileName, ".as");
    
    if (!(srcFile = fopen("src.as", "r"))) {
        
        printError("can't open the source file");
        exit(0);
    }
    
    while (!endOfSrc) {
        
        readedFirstWord = readFirstWord(&srcFile);
        
    }
    
    return 0;
}

