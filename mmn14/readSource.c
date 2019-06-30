
#include <stdio.h>
#include <stdlib.h>
#include "readSource.h"
#include "utils.h"

void ignoreWhiteChar(FILE** file);

int readFirstWord(FILE** file) {
    
    int charCount = 0;
    
    /* check if the line is not too long */
    while (fgetc(*file) != '\n' && !feof(*file)) {
        charCount++;
    }
    
    if (charCount >= MAX_LINE) {
        
        printError("the line is too long");
        exit(0);
    }
    
    ignoreWhiteChar(&(*file));
    
    /* chek if the line is a comment */
    if (getc(*file) == ';')
        return in_comment;
    
    return 0;
}

void ignoreWhiteChar(FILE** file) {
    
    while ((fgetc(*file) == ' ' || fgetc(*file) == '\t') && !feof(*file)) {
    };
    
    fseek(*file, -1, SEEK_CUR);
}
