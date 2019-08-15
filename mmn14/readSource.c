
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
void moveWordBack(FILE** file, char charLimit);
void readNextWord(FILE** src, char* dest, char charLimit);
int isLegalOptChar(char* word);
int isReserved(char* word);
int isNumber(char* word);
int islegalMacroName(char* macroName);
adOperand readOperand(FILE** file, int isSrcOp);
void resetInstruct(instructField* instruction);

int readFirstWord(FILE** file, char* readedWord) {
    
    int charCount = 0;
    char word[MAX_MACRO_SIZE + 1] = {};
    fpos_t lineBegining;
    char readedChar;
    
    fgetpos(*file, &lineBegining);
    
    /* check if the line is not too long */
    for (charCount = 0; fgetc(*file) != '\n' && !feof(*file); charCount++);
    
    if (charCount > MAX_LINE_SIZE) {
        
        printErrorInSrcFile("the line is too long");
        return 0;
    }

    fsetpos(*file, &lineBegining);
    ignoreWhiteChar(&(*file));
    readedChar = fgetc(*file);
    
    if (readedChar == EOF)
        return end_src_file;
    
    if (readedChar == '\n') {
        
        moveBack(&(*file));
        return blank_line;
    }
    
    /* chek if the line is a comment */
    if (readedChar == ';')
        return in_comment;
    
    moveBack(&(*file));
    
    /* count the word length */
    for (charCount = 0; fgetc(*file) != ' '; charCount++);
    
    if (charCount > MAX_MACRO_SIZE) {
        
        printErrorInSrcFile("optianal charactere is to long");
        return -1;
    }
    
    fsetpos(*file, &lineBegining);
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
            
            if (!feof(*src))
                moveBack(&(*src));
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
    
    char word[MAX_LINE_SIZE] = {};
    int macroVal = 0;
    
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
    
    if (!haveErrorInLine) {
        
        readNextWord(&(*file), word, '\0');
        
        if (!isNumber(word))
            printErrorInSrcFile("illegal macro value");
        
        ignoreWhiteChar(&(*file));
        
        if (fgetc(*file) != '\n' && !feof(*file))
            printErrorInSrcFile("extra end line text");
    }
    
    if (!haveErrorInLine) {
        
        macroVal = atoi(word);
        
        if (macroVal > MAX_VAL || macroVal < MIN_VAL)
            printErrorInSrcFile("your macro number is to big or to small");
    }
    
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
    resetInstruct(&instruction);
    
    char actualChar;
    
    if (strcmp(instructName, "") != 0)
        strcpy(instruction.name, instructName);
    
    else
        strcpy(instruction.name, "");
    
    instruction.type = instructType;
    
    /* the instruction there should have source operand */
    if (instructType == inst_mov || instructType == inst_cmp || instructType == inst_add ||
        instructType == inst_sub || instructType == inst_lea) {
        
        instruction.srcOp = readOperand(&(*file), 1);
        
        if (instruction.type == inst_lea && (instruction.srcOp.type == imediate_met ||
                                             instruction.srcOp.type == register_met)) {
            
            printErrorInSrcFile("the imediate and register source operand methode are illegal with 'lea' instruction");
        }
        
        /* check if there is not a destination */
        ignoreWhiteChar(&(*file));
        actualChar = fgetc(*file);
        
        if (actualChar == '\n') {
            
            printErrorInSrcFile("expected destination operand");
            moveBack(&(*file));
        }
        
        else if (actualChar != ',')
            printErrorInSrcFile("missing coma after the source operand");
    }
    
    if (!haveErrorInLine && instructType != inst_rts && instructType != inst_stop) {
        
        instruction.destOp = readOperand(&(*file), 0);
        
        if (instruction.destOp.type == imediate_met && instructType != inst_cmp &&
            instructType != inst_prn && !haveError) {
            
            printErrorInSrcFile("the destination imediate operand method is illegal with your instruction");
        }
        
        if (instruction.destOp.type == index_met && (instructType == inst_jmp ||
                                                     instructType == inst_bne ||
                                                     instructType == inst_jsr)) {
            
            printErrorInSrcFile("the destination index operand method is illegal with your instruction");
        }
    }
    
    if (!haveErrorInLine) {
        
        ignoreWhiteChar(&(*file));
        actualChar = fgetc(*file);
        
        if (actualChar != '\n' && !feof(*file))
            printErrorInSrcFile("extra end text");
        
        if (!feof(*file))
            moveBack(&(*file));
    }
    
    return instruction;
}

adOperand readOperand(FILE** file, int isSrcOp) {
    
    adOperand operand;
    char actualChar;
    char readedWord[MAX_LINE_SIZE] = {};
    int tmp;
    
    ignoreWhiteChar(&(*file));
    actualChar = fgetc(*file);
    
    /* initialize operand */
    operand.type = 0;
    operand.val = 0;
    strcpy(operand.macroName, "");
    strcpy(operand.indexName, "");
    
    if (actualChar == '\n') {
        
        if (isSrcOp)
            printErrorInSrcFile("missing source and destination operand");
        
        else
            printErrorInSrcFile("missing destination operand");
        
        moveBack(&(*file));
    }
    
    else if (actualChar == '#') {
        
        if (isSrcOp)
            readNextWord(&(*file), readedWord, ',');
        
        else
            readNextWord(&(*file), readedWord, '\0');
        
        if (!isNumber(readedWord)) {
            
            if (!islegalMacroName(readedWord))
                printErrorInSrcFile("imediate value is not a valid number or macro name");
            
            /* the value is a macro */
            if (!haveErrorInLine) {
                
                if (strlen(readedWord) > MAX_MACRO_SIZE)
                    printErrorInSrcFile("the macro name is too big");
                
                else {
                    
                    operand.type = imediate_met;
                    strcpy(operand.macroName, readedWord);
                }
            }
        }
        
        /* the value is a number */
        else {
            
            operand.type = imediate_met;
            operand.val = atoi(readedWord);
        }
    }
    
    else if (isalpha(actualChar)) {
        
        int isRegister = 0, OpIsReaded = 0;
        
        if (actualChar == 'r') {
            
            if (isSrcOp)
                readNextWord(&(*file), readedWord, ',');
            
            else
                readNextWord(&(*file), readedWord, '\0');
            
            OpIsReaded = 1;
            
            if (strlen(readedWord) == 1) {
                
                tmp = atoi(readedWord);
                
                /* if true: the source operand is a register */
                if (strcmp(readedWord, "0") == 0 || (tmp >= 1 && tmp <= 7)) {
                    
                    isRegister = 1;
                    operand.type = register_met;
                    operand.val = tmp;
                }
            }
        }
        
        if (!isRegister) {
            
            char *indexStr;
            
            if (isSrcOp) {
                
                moveWordBack(&(*file), '\0');
                readNextWord(&(*file), readedWord, ',');
            }
            
            else {
                
                moveWordBack(&(*file), ',');
                readNextWord(&(*file), readedWord, '\0');
            }
            
            /* check if ']' is in the word */
            indexStr = strchr(readedWord, ']');
            
            /* check if we find the close bracket and if the bracket is at the end of the word */
            if (indexStr != NULL && *(indexStr + 1) == '\0') {
                
                /* erase the close brackets to check the number inside */
                *indexStr = '\0';
                
                /* check if ']' is in the word */
                indexStr = strchr(readedWord, '[');
                
                if (indexStr != NULL) {
                    
                    /* erase the '[' in the word for get the name of the
                     optional character */
                    *indexStr = '\0';
                    
                    /* move indexStr inside the brackets */
                    indexStr = indexStr + 1;
                    
                    if (isNumber(indexStr)) {
                        
                        tmp = atoi(indexStr);
                        
                        if (tmp < 0)
                            printErrorInSrcFile("invalid index number, the index have to be positive");
                        
                        else {
                            
                            if (strlen(readedWord) > MAX_MACRO_SIZE)
                                printErrorInSrcFile("the optional charactere name is too big");
                            
                            else {
                                
                                operand.type = index_met;
                                operand.val = tmp;
                                strcpy(operand.macroName, readedWord);
                            }
                        }
                    }
                    
                    else if (islegalMacroName(indexStr)) {
                        
                        if (strlen(readedWord) > MAX_MACRO_SIZE)
                            printErrorInSrcFile("the macro name in index is too big");
                        
                        else {
                            
                            operand.type = index_met;
                            strcpy(operand.macroName, readedWord);
                            strcpy(operand.indexName, indexStr);
                        }
                    }
                    
                    else
                        printErrorInSrcFile("illegal index name");
                }
                
                else
                    printErrorInSrcFile("illegal optional character name");
            }
            
            else if (islegalMacroName(readedWord)) {
                
                if (strlen(readedWord) > MAX_MACRO_SIZE)
                    printErrorInSrcFile("the optional charactere name is too big");
                
                else {
                    
                    operand.type = direct_met;
                    strcpy(operand.macroName, readedWord);
                }
            }
            
            else
                printErrorInSrcFile("illegal optional character name");
        }
    } /* end of: else if (isalpha(actualChar)) */
    
    else
        printErrorInSrcFile("illegal optional character name");
    
    return operand;
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
    
    /* check if the first character is '+' or '-' and return 0 if there is no
       number after the '+' or '-' */
    if (word[0] == '+' || word[0] == '-') {
        
        if (!isdigit(word[1]))
            return 0;
    }
    
    else if (!isdigit(word[0]))
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
    
    if (!feof(*file))
        moveBack(&(*file));
}

/* used after fgetc to go back one character */
void moveBack(FILE** file) {
    
    fseek(*file, -1, SEEK_CUR);
}

/* move to a word begining */
void moveWordBack(FILE** file, char charLimit) {
    
    int beginPoint = 0;
    int actualChar;
    
    do {
        
        moveBack(&(*file));
        actualChar = fgetc(*file);
        
        if (actualChar == ' ' || actualChar == '\t' || actualChar == '\n' || actualChar == charLimit)
            beginPoint = 1;
        
        moveBack(&(*file));
    }while (!beginPoint);
    
    /* move to word begining char */
    fgetc(*file);
}

void mvToNextLine(FILE** file) {
    
    int readedChar;
    
    do {
        
        readedChar = fgetc(*file);
    }while (readedChar != '\n' && readedChar != EOF);
    
        
}

void resetInstruct(instructField* instruction) {
    
    strcpy(instruction->name, "");
    instruction->type = 0;
    instruction->srcOp.type = 0;
    instruction->srcOp.val = 0;
    strcpy(instruction->srcOp.macroName, "");
    strcpy(instruction->srcOp.indexName, "");
    instruction->destOp.type = 0;
    instruction->destOp.val = 0;
    strcpy(instruction->destOp.macroName, "");
    strcpy(instruction->destOp.indexName, "");
    instruction->ARE = 0;
}
