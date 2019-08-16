
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"

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
    instructField actualInstruct;
    
    if (strlen(argv[1]) >= MAX_FILE_NAME)
        printError("the source file name is to long");
    
    strcpy(srcFileName, argv[1]);
    strcat(srcFileName, ".as");
    
    if (!(srcFile = fopen("src.as", "r")))
        printError("can't open the source file");
    
    /* construct the signTabHead */
    signTabCtor(&signTabHead);
    
    /* first passage */
    while (!endOfSrc) {
        
        firstWordType = readFirstWord(srcFile, readedFirstWord);
        
        if (firstWordType == optional_char) {
            
            haveOptChar = 1;
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
        }
        
        else if (firstWordType == data_line) {
            
            int endOfLine = 1;
            
            while (!endOfLine && !haveErrorInLine) {
                
                
            }
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
    
    freeSignTab(signTabHead);
    
    return 0;
}


