
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readSource.h"
#include "utils.h"

/* private functions */
void ignoreWhiteChar(FILE** file);
void moveBack(FILE** file);
void readNextWord(FILE** src, char* dest);
int isInstruction(char* word);
int isLegalOptChar(char* word);
int isReserved(char* word);
int isNumber(char* word);

int readFirstWord(FILE** file) {
    
    int charCount = 0;
    char word[MAX_LINE + 1] = {};
    
    /* check if the line is not too long */
    while (fgetc(*file) != '\n' && !feof(*file)) {
        charCount++;
    }
    
    if (charCount > MAX_LINE)
        printError("the line is too long");
    
    rewind(*file);
    ignoreWhiteChar(&(*file));
    
    /* chek if the line is a comment */
    if (fgetc(*file) == ';')
        return in_comment;
    
    moveBack(&(*file));
    readNextWord(&(*file), word);
    
    if (strcmp(word, ".define") == 0)
        return in_macro;
    
    if (strcmp(word, ".entry") == 0)
        return entry_line;
    
    if (strcmp(word, ".extern") == 0)
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
    
    if (strcmp(word, "mov") == 0 || strcmp(word, "cmp") == 0 || strcmp(word, "add") == 0 ||
        strcmp(word, "sub") == 0 || strcmp(word, "not") == 0 || strcmp(word, "clr") == 0 ||
        strcmp(word, "lea") == 0 || strcmp(word, "inc") == 0 || strcmp(word, "dec") == 0 ||
        strcmp(word, "jmp") == 0 || strcmp(word, "bne") == 0 || strcmp(word, "red") == 0 ||
        strcmp(word, "prn") == 0 || strcmp(word, "jsr") == 0 || strcmp(word, "rts") == 0 ||
        strcmp(word, "stop") == 0) {
        
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
    while (word[i] != ':' && word[i] != '\0') {
        
        if (!isalpha(word[i]) && !isdigit(word[i]))
            return 0;
        
        i++;
    }
    
    /* check if the last char is ':' */
    if (word[i] == ':') {
        
        if (word[i + 1] != '\0') /* the ':' is not the last character */
            return 0;
    }
    
    else /* missing ':' */
        return 0;
    
    return 1;
}

int readMacro(FILE** file) {
    
    int i = 1;
    char word[MAX_LINE] = {};
    
    readNextWord(&(*file), word);
    
    if (isReserved(word))
        printError("the macro name is reserved");
    
    /* check if the first character is a letter */
    if (!isalpha(word[0]))
        printError("illegal macro name");
    
    /* check if next charcter is a letter or a digit */
    while (word[i] != '\0') {
        
        if (!isalpha(word[i]) && !isdigit(word[i]))
            printError("illegal macro name");
    }
    
    readNextWord(&(*file), word);
    
    if (strcmp(word, "=") != 0)
        printError("wrong macro declaration");
    
    readNextWord(&(*file), word);
    
    if (!isNumber(word))
        printError("illegal macro value");
    
    ignoreWhiteChar(&(*file));
    
    if (fgetc(*file) != '\n' && !feof(*file))
        printError("extra end line text");
    
    return 1;
}

int isReserved(char* word) {
    
    if (isInstruction(word))
        return 1;
    
    if (strcmp(word, "r0") == 0 || strcmp(word, "r1") == 0 || strcmp(word, "r2") == 0 ||
        strcmp(word, "r3") == 0 || strcmp(word, "r4") == 0 || strcmp(word, "r5") == 0 ||
        strcmp(word, "r6") == 0 || strcmp(word, "r7") == 0)
        return 1;
    
    return 0;
}

int isNumber(char* word){
    
    int i = 1;
    
    /* check if the first character is a digit, + ot - */
    if (!isdigit(word[0]) && word[0] != '+' && word[0] != '-')
        return 0;
    
    /* check if all charcters is a digit */
    while (word[i] != '\0') {
        
        if (!isdigit(word[i]))
            return 0;
        
        i++;
    }
    
    return 1;
}

void ignoreWhiteChar(FILE** file) {
    
    char currentChar;
    
    do {
        currentChar = fgetc(*file);
    }while (currentChar == ' ' || currentChar == '\t');
    
    moveBack(&(*file));
}

/* used after fgetc to go back one character */
void moveBack(FILE** file) {
    
    fseek(*file, -1, SEEK_CUR);
}
