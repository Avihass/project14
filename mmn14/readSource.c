
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readSource.h"
#include "utils.h"

void ignoreWhiteChar(FILE** file);
void moveBack(FILE** file);
void readNextWord(FILE** src, char* dest);
int isInstruction(char* word);
int isLegalOptChar(char* word);

int readFirstWord(FILE** file) {
    
    int charCount = 0;
    char word[MAX_LINE] = {};
    
    /* check if the line is not too long */
    while (fgetc(*file) != '\n' && !feof(*file)) {
        charCount++;
    }
    
    if (charCount >= MAX_LINE)
        printError("the line is too long");
    
    ignoreWhiteChar(&(*file));
    
    /* chek if the line is a comment */
    if (fgetc(*file) == ';')
        return in_comment;
    
    moveBack(&(*file));
    readNextWord(&(*file), word);
    
    if (strcmp(word, ".define"))
        return in_macro;
    
    if (strcmp(word, ".entry"))
        return entry_line;
    
    if (strcmp(word, ".extern"))
        return extern_line;
    
    if (isInstruction(word))
        return instruction_line;
    
    if (isLegalOptChar(word))
        return optional_char;
    
    printError("illegal optional character");
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

int isInstruction(char* word) {
    
    if (strcmp(word, "mov") || strcmp(word, "cmp") || strcmp(word, "add") ||
        strcmp(word, "sub") || strcmp(word, "not") || strcmp(word, "clr") ||
        strcmp(word, "lea") || strcmp(word, "inc") || strcmp(word, "dec") ||
        strcmp(word, "jmp") || strcmp(word, "bne") || strcmp(word, "red") ||
        strcmp(word, "prn") || strcmp(word, "jsr") || strcmp(word, "rts") ||
        strcmp(word, "stop")) {
        
        return 1;
    }
    
    return 0;
}

int isLegalOptChar(char* word) {
    
    int i = 1;
    
    /* check if the first character is a letter */
    if (!isalpha(word[0]))
        return 0;
    
    /* check if next charcter is a letter or a digit */
    while (word[i] != '\0') {
        
        if (!isalpha(word[i]) && !isdigit(word[i]))
            return 0;
    }
    
    /* check if the last char is ':' */
    if (word[i - 1] != ':')
        return 0;
    
    return 1;
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
