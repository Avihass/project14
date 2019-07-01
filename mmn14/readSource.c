
#include <stdio.h>
#include <stdlib.h>
#include "readSource.h"
#include "utils.h"

void ignoreWhiteChar(FILE** file);
void moveBack(FILE** file);

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
    if (fgetc(*file) == ';')
        return in_comment;
    
    return 0;
}

void readNextWord(FILE** src, char* dest) {
    
    int inWord = 1;
    char currentChar;
    int i = 0;
    
    ignoreWhiteChar(&(*src));
    
    while (inWord) {
        
        currentChar = fgetc(*src);
        
        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n'
            || feof(*src)) {
            
            inWord = 0;
        }
        
        else {
            
            dest[i] = currentChar;
            i++;
        }
    }
}

void ignoreWhiteChar(FILE** file) {
    
    while ((fgetc(*file) == ' ' || fgetc(*file) == '\t') && !feof(*file)) {
    };
    
    fseek(*file, -1, SEEK_CUR);
}

/* used after fgetc to go back one character */
void moveBack(FILE** file) {
    
    fseek(*file, -1, SEEK_CUR);
}
