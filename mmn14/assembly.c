
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"
#include "bin&specConvert.h"

#define GO_NEXT_LINE \
    mvToNextLine(srcFile); \
    actLineInSrc++; \
    haveErrorInLine = 0; \
    haveOptChar = 0;

#define CHECK_INSTRUCT_MACRO(OP) \
{ \
    int tmpNum; \
    if (actualInstruct.OP.type == imediate_met) { \
 \
        if (strcmp(actualInstruct.OP.macroName, "") != 0) { \
 \
            if (!findMacro(signTabHead, actualInstruct.OP.macroName, &tmpNum)) \
                printErrorInSrcFile("used undeclared macro name"); \
        } \
    } \
 \
    else if(actualInstruct.OP.type == index_met) { \
 \
        if (strcmp(actualInstruct.OP.indexName, "") != 0) { \
\
            if (!findMacro(signTabHead, actualInstruct.OP.indexName, &tmpNum)) \
                printErrorInSrcFile("used undeclared macro name"); \
        } \
    } \
}

/* the actual line in source file number */
int actLineInSrc = 1;

int main(int argc, const char * argv[]) {
    
    char srcFileName[MAX_FILE_NAME + 4] = {}; /* +4 character for ".as" and '\0' */
    FILE *srcFile;
    int firstWordType;
    char readedFirstWord[MAX_MACRO_SIZE] = {};
    int endOfSrc = 0;
    char macroName[MAX_MACRO_SIZE] = {};
    int macroVal;
    char optCharName[MAX_MACRO_SIZE] = {};
    int haveOptChar = 0;
    signTabPtr signTabHead;
    dataTabPtr dataTabHead;
    instructField actualInstruct;
    binAdressWord actualAdWord;
    int IC, DC;
    
    if (strlen(argv[1]) >= MAX_FILE_NAME)
        printError("the source file name is to long");
    
    strcpy(srcFileName, argv[1]);
    strcat(srcFileName, ".as");
    
    if (!(srcFile = fopen("src.as", "r")))
        printError("can't open the source file");
    
    /* construct the tables */
    signTabCtor(&signTabHead);
    dataTabCtor(&dataTabHead);
    
    IC = 100;
    DC = 0;
    
    /* first passage */
    while (!endOfSrc) {
        
        firstWordType = readFirstWord(srcFile, readedFirstWord);
        
        if (firstWordType == optional_char) {
            
            haveOptChar = 1;
            
            /* remove the ':' character */
            readedFirstWord[strlen(readedFirstWord) - 1] = '\0';
            
            strcpy(optCharName, readedFirstWord);
            firstWordType = readFirstWord(srcFile, readedFirstWord);
        }
        
        if (firstWordType == in_macro) {
            
            macroVal = readMacro(srcFile, macroName);
            
            if (haveOptChar)
                printErrorInSrcFile("optional char are illegal with macro statement");
            
            if (!haveErrorInLine) {
                
                if (!isAvailableSign(signTabHead, macroName))
                    printErrorInSrcFile("the macro name is not available");
                
                else
                    addSign(signTabHead, macroName, macro_sign, macroVal);
            }
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == data_line) {
            
            int endOfLine = 0;
            int dataNum;
            char macroName[MAX_MACRO_SIZE] = {};
            
            if (haveOptChar) {
                
                if (isAvailableSign(signTabHead, optCharName))
                    addSign(signTabHead, optCharName, data_sign, DC);
                
                else
                    printErrorInSrcFile("the optioanal charactere name is not available");
            }
            
            while (!endOfLine && !haveErrorInLine) {
                
                dataNum = readDataDirective(srcFile, macroName, &endOfLine);
                
                /* if the maro is not empty, the data is a macro */
                if (strcmp(macroName, "")) {
                    
                    if (!findMacro(signTabHead, macroName, &dataNum))
                        printErrorInSrcFile("used undeclared macro name");
                }
                
                if (!haveErrorInLine) {
                    
                    /* insert the number to the data table, in adress word */
                    resetBinWord(actualAdWord);
                    insrtDecToBin(actualAdWord, dataNum, 0, BIN_WORD_SIZE);
                    addData(dataTabHead, DC, actualAdWord);
                    DC++;
                }
            }
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == string_line) {
            
            char readedStr[MAX_LINE_SIZE];
            int i;
            
            if (haveOptChar) {
                
                if (isAvailableSign(signTabHead, optCharName))
                    addSign(signTabHead, optCharName, data_sign, DC);
                
                else
                    printErrorInSrcFile("the optioanal charactere name is not available");
            }
            
            readStringDirective(srcFile, readedStr);
            
            /* if the string is not empty */
            if (strcmp(readedStr, "") != 0 && !haveErrorInLine) {
                
                for (i = 0; i < strlen(readedStr); i++) {
                    
                    resetBinWord(actualAdWord);
                    insrtDecToBin(actualAdWord, readedStr[i], 0, BIN_WORD_SIZE);
                    addData(dataTabHead, DC, actualAdWord);
                    DC++;
                }
            }
            
            /* add '\0' at the end even if the string is empty */
            if (!haveErrorInLine) {
                
                resetBinWord(actualAdWord);
                addData(dataTabHead, DC, actualAdWord);
                DC++;
            }
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == entry_line) {
            
            char readedEntry[MAX_MACRO_SIZE];
            
            if (haveOptChar)
                printf("Warning: optional char before entry line have no meaning\n\n");
            
            /* just check the line syntax */
            readEntryOrExtern(srcFile, readedEntry);
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == extern_line) {
            
            char readedExtern[MAX_MACRO_SIZE];
            
            if (haveOptChar)
                printf("Warning: optional char before extern line have no meaning\n\n");
            
            readEntryOrExtern(srcFile, readedExtern);
            
            if (!haveErrorInLine)
                addSign(signTabHead, readedExtern, external_sign, 0);
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == instruction_line) {
            
            if (haveOptChar) {
                
                if (isAvailableSign(signTabHead, optCharName)) {
                    
                    addSign(signTabHead, optCharName, instruct_sign, IC);
                }
                
                else
                    printErrorInSrcFile("the optioanal charactere name is not available");
            }
            
            actualInstruct.type = identifyInstruction(readedFirstWord);
            actualInstruct = readInstruction(srcFile, actualInstruct.type);
            
            /* check if used undeclared macro */
            CHECK_INSTRUCT_MACRO(srcOp)
            CHECK_INSTRUCT_MACRO(destOp)
            
            IC += instructAdCount(actualInstruct);
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == blank_line || firstWordType == in_comment) {
            
            if (haveOptChar)
                printErrorInSrcFile("expected directive or instruction after optional char");
            
            GO_NEXT_LINE
        }
        
        else if (firstWordType == end_src_file) {
            
            if (haveOptChar)
                printErrorInSrcFile("expected directive or instruction after optional char");
            
            endOfSrc = 1;
        }
        
        printf("%d %d \n", IC, DC);
    }
    
    freeSignTab(signTabHead, NULL);
    freeDataTab(dataTabHead, NULL);
    
    return 0;
}


