
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readSource.h"
#include "utils.h"
#include "assembly.h"

/* private functions */
void ignoreWhiteChar(FILE** file);
void moveBack(FILE** file);
void readNextWord(FILE** src, char* dest, char charLimit);
int isLegalOptChar(char* word);
int isReserved(char* word);
int isNumber(char* word);
int islegalMacroName(char* macroName);

int readFirstWord(FILE** file, char* readedWord) {
    
    int charCount = 0;
    char word[MAX_LINE + 1] = {};
    fpos_t lineBegining;
    char readedChar;
    
    fgetpos(*file, &lineBegining);
    
    /* check if the line is not too long */
    while (fgetc(*file) != '\n' && !feof(*file)) {
        charCount++;
    }
    
    if (charCount > MAX_LINE) {
        
        printErrorInSrcFile("the line is too long");
        return 0;
    }

    fsetpos(*file, &lineBegining);
    
    ignoreWhiteChar(&(*file));
    
    /* move forward to check EOF */
    fseek(*file, 1, SEEK_CUR);
    readedChar = fgetc(*file);
    
    if (readedChar == EOF)
        return end_src_file;
    
    fsetpos(*file, &lineBegining);
    readedChar = fgetc(*file);
    
    if (readedChar == '\n')
        return blank_line;
    
    /* chek if the line is a comment */
    if (readedChar == ';')
        return in_comment;
    
    moveBack(&(*file));
    
    readNextWord(&(*file), word, '\0');
    
    if (strcmp(word, ".define") == 0)
        return in_macro;
    
    if (strcmp(word, ".entry") == 0)
        return entry_line;
    
    if (strcmp(word, ".extern") == 0)
        return extern_line;
    
    if (identifyInstruction(word) != -1) {
        
        strcpy(readedWord, word);
        return instruction_line;
    }
    
    if (isLegalOptChar(word)) {
        
        strcpy(readedWord, word);
        return optional_char;
    }
    
    printErrorInSrcFile("illegal optional character");
    return -1;
}

/* the function stop to read when we read ' ' (space), '\t' or eof and
   receive charLimit in parameter for stop read when we read another
   character, we can send '\0' if there is no specifie character */
void readNextWord(FILE** src, char* dest, char charLimit) {
    
    int inWord = 1;
    char currentChar;
    int i = 0;
    
    /* reset the destination string */
    for (i = 0; dest[i] != '\0'; i++) {
        dest[i] = '\0';
    }
    
    i = 0;
    ignoreWhiteChar(&(*src));
    
    while (inWord) {
        
        currentChar = fgetc(*src);
        
        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n'
            || currentChar == charLimit || feof(*src)) {
            
            inWord = 0;
        }
        
        else {
            
            dest[i] = currentChar;
            i++;
        }
    }
}

int identifyInstruction(char* word) {
    
    if (strcmp(word, "mov") == 0)
        return inst_mov;
    
    if (strcmp(word, "cmp") == 0)
        return inst_cmp;
    
    if (strcmp(word, "add") == 0)
        return inst_add;
    
    if (strcmp(word, "sub") == 0)
        return inst_sub;
    
    if (strcmp(word, "not") == 0)
        return inst_not;
    
    if (strcmp(word, "clr") == 0)
        return inst_clr;
    
    if (strcmp(word, "lea") == 0)
        return inst_lea;
    
    if (strcmp(word, "inc") == 0)
        return inst_inc;
    
    if (strcmp(word, "dec") == 0)
        return inst_dec;
    
    if (strcmp(word, "jmp") == 0)
        return inst_jmp;
    
    if (strcmp(word, "bne") == 0)
        return inst_bne;
    
    if (strcmp(word, "red") == 0)
        return inst_red;
    
    if (strcmp(word, "prn") == 0)
        return inst_prn;
    
    if (strcmp(word, "jsr") == 0)
        return inst_jsr;
    
    if (strcmp(word, "rts") == 0)
        return inst_rts;
    
    if (strcmp(word, "stop") == 0)
        return inst_stop;
    
    return -1;
}

int isLegalOptChar(char* optCharName) {
    
    int i = 1;
    
    /* check if the first character is a letter */
    if (!isalpha(optCharName[0]))
        return 0;
    
    /* check if next charcter is a letter or a digit */
    while (optCharName[i] != ':' && optCharName[i] != '\0') {
        
        if (!isalpha(optCharName[i]) && !isdigit(optCharName[i]))
            return 0;
        
        i++;
    }
    
    /* check if the last char is ':' */
    if (optCharName[i] == ':') {
        
        if (optCharName[i + 1] != '\0') /* the ':' is not the last character */
            return 0;
    }
    
    else /* missing ':' */
        return 0;
    
    return 1;
}

int readMacro(FILE** file, char* macroName) {
    
    char word[MAX_LINE] = {};
    int macroVal;
    
    readNextWord(&(*file), word, '\0');
    
    if (isReserved(word))
        printErrorInSrcFile("the macro name is reserved");
    
    if (!islegalMacroName(word)) {
        printErrorInSrcFile("illegal macro name");
    }
    
    strcpy(macroName, word);
    
    readNextWord(&(*file), word, '\0');
    
    if (strcmp(word, "=") != 0)
        printErrorInSrcFile("wrong macro declaration");
    
    if (!haveError) {
        
        readNextWord(&(*file), word, '\0');
        
        if (!isNumber(word))
            printErrorInSrcFile("illegal macro value");
        
        ignoreWhiteChar(&(*file));
        
        if (fgetc(*file) != '\n' && !feof(*file))
            printErrorInSrcFile("extra end line text");
    }
    
    if (haveError)
        macroVal = 0;
    
    else
        macroVal = atoi(word);
    
    return macroVal;
}

int islegalMacroName(char* macroName) {
    
    int i;
    
    /* check if the first character is a letter */
    if (!isalpha(macroName[0]))
        return 0;
        
        /* check if next charcter is a letter or a digit */
    for (i = 1; macroName[i] != '\0'; i++) {
            
        if (!isalpha(macroName[i]) && !isdigit(macroName[i]))
            return 0;
    }
    
    return 1;
}

instructField readInstruction(FILE** file, char* instructName, int instructType) {
    
    instructField instruction;
    char actualChar;
    char readedWord[MAX_LINE];
    
    if (strcmp(instructName, "") != 0)
        strcpy(instruction.name, instructName);
    
    else
        strcpy(instruction.name, "");
    
    instruction.type = instructType;
    
    /* the instruction there should have source operand */
    if (instructType == inst_mov || instructType == inst_cmp || instructType == inst_add ||
        instructType == inst_sub || instructType == inst_lea) {
        
        /* read source operand method */
        ignoreWhiteChar(&(*file));
        actualChar = fgetc(*file);
        
        if (actualChar == '#') {
            
            readNextWord(&(*file), readedWord, ',');
            
            if (!isNumber(readedWord)) {
                
                if (!islegalMacroName(readedWord))
                    printErrorInSrcFile("imediate value is not a valid number or macro name");
                
                /* the value is a macro */
                if (!haveError) {
                    instruction.srcOp.type =  imediate_met;
                    instruction.srcOp.val = 0;
                    strcpy(instruction.srcOp.macroName, readedWord);
                    instruction.ARE = 0;
                }
            }
            
            /* the value is a number */
            else {
                
                instruction.srcOp.type =  imediate_met;
                instruction.srcOp.val = atoi(readedWord);
                strcpy(instruction.srcOp.macroName, "");
                instruction.ARE = 0;
            }
        }
        
        else if (actualChar == 'r') {
            
            readNextWord(&(*file), readedWord, ',');
            
        }
    }
    
    return instruction;
}

int isReserved(char* word) {
    
    if (identifyInstruction(word))
        return 1;
    
    if (strcmp(word, "r0") == 0 || strcmp(word, "r1") == 0 || strcmp(word, "r2") == 0 ||
        strcmp(word, "r3") == 0 || strcmp(word, "r4") == 0 || strcmp(word, "r5") == 0 ||
        strcmp(word, "r6") == 0 || strcmp(word, "r7") == 0)
        return 1;
    
    return 0;
}

int isNumber(char* word){
    
    int i = 1;
    
    /* check if the first character is a digit, '+' or '-' */
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

void mvToNextLine(FILE** file) {
    
    char readedChar;
    
    do {
        
        readedChar = fgetc(*file);
    }while (readedChar != '\n' && readedChar != EOF);
    
        
}
