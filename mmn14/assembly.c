
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"
#include "bin&specConvert.h"

/* the actual line in source file number */
int actLineInSrc = 1;

int main(int argc, const char * argv[]) {
    
    char srcFileName[MAX_FILE_NAME + 4] = {}; /* +4 character for ".as" and '\0' */
    FILE *srcFile;
    int firstWordType, instructType;
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
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
            haveOptChar = 0;
        }
        
        else if (firstWordType == data_line) {
            
            int endOfLine = 0;
            int dataNum;
            char macroName[MAX_MACRO_SIZE] = {};
            
            if (haveOptChar) {
                
                if (isAvailableSign(signTabHead, optCharName))
                    addSign(signTabHead, optCharName, data_sign, DC);
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
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
            haveOptChar = 0;
        }
        
        else if (firstWordType == string_line) {
            
            char readedStr[MAX_LINE_SIZE];
            int i;
            
            if (haveOptChar) {
                
                if (isAvailableSign(signTabHead, optCharName))
                    addSign(signTabHead, optCharName, data_sign, DC);
            }
            
            readStringDirective(srcFile, readedStr);
            
            /* if the string is not empty */
            if (readedStr[0] != '\0') {
                
                for (i = 0; i < strlen(readedStr); i++) {
                    
                    resetBinWord(actualAdWord);
                    insrtDecToBin(actualAdWord, readedStr[i], 0, BIN_WORD_SIZE);
                    addData(dataTabHead, DC, actualAdWord);
                    DC++;
                }
            }
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
            haveOptChar = 0;
        }
        
        else if (firstWordType == entry_line) {
            
            char readedEntry[MAX_MACRO_SIZE];
            
            if (haveOptChar)
                printf("Warning: optional char before entry line have no meaning");
            
            /* just check the line syntax */
            readEntryOrExtern(srcFile, readedEntry);
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
            haveOptChar = 0;
        }
        
        else if (firstWordType == extern_line) {
            
            char readedExtern[MAX_MACRO_SIZE];
            
            if (haveOptChar)
                printf("Warning: optional char before extern line have no meaning");
            
            readEntryOrExtern(srcFile, readedExtern);
            
            if (!haveErrorInLine)
                addSign(signTabHead, readedExtern, external_sign, 0);
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
            haveOptChar = 0;
        }
        
        else if (firstWordType == instruction_line) {
            
            instructType = identifyInstruction(readedFirstWord);
            actualInstruct = readInstruction(srcFile, "", instructType);
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
        }
        
        else if (firstWordType == blank_line || firstWordType == in_comment) {
            
            if (haveOptChar)
                printErrorInSrcFile("expected directive or instruction after optional char");
            
            mvToNextLine(srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
        }
        
        else if (firstWordType == end_src_file) {
            
            if (haveOptChar)
                printErrorInSrcFile("expected directive or instruction after optional char");
            
            endOfSrc = 1;
        }
    }
    
    freeSignTab(signTabHead, NULL);
    freeDataTab(dataTabHead, NULL);
    
    return 0;
}


