
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
    
    char srcFileName[MAX_FILE_NAME + 4]; /* +4 character for ".as" and '\0' */
    FILE *srcFile;
    int firstWordType, instructType;
    char readedFirstWord[MAX_MACRO_SIZE];
    int endOfSrc = 0;
    char macroName[MAX_MACRO_SIZE];
    int macroVal;
    signTabPtr signTabHead;
    instructField actualInstruct;
    
    if (strlen(argv[1]) >= MAX_FILE_NAME)
        printError("the source file name is to long");
    
    strcpy(srcFileName, argv[1]);
    strcat(srcFileName, ".as");
    
    if (!(srcFile = fopen("src.as", "r")))
        printError("can't open the source file");
    
    signTabCtor(&signTabHead);
    
    /* first read */
    while (!endOfSrc) {
        
        firstWordType = readFirstWord(&srcFile, readedFirstWord);
        printf("the first word is: %d\n", firstWordType);
        
        if (firstWordType == in_macro) {
            
            macroVal = readMacro(&srcFile, macroName);
            
            if (!haveErrorInLine) {
                
                if (!isAvailable(signTabHead, macroName))
                    printErrorInSrcFile("the macro name is not available");
            }
            
            if (!haveError) {
                addSign(signTabHead, macroName, macro_sign, macroVal);
                
                printf("the name of the macro: %s\n", signTabHead->sign);
                printf("the value: %d\n", signTabHead->value);
            }
            
            mvToNextLine(&srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
        }
        
        else if (firstWordType == instruction_line) {
            
            instructType = identifyInstruction(readedFirstWord);
            actualInstruct = readInstruction(&srcFile, "", instructType);
            
            printf("instruct type: %d\n", actualInstruct.type);
            printf("\nsource operand type: %d\n", actualInstruct.srcOp.type);
            printf("source operand value: %d\n", actualInstruct.srcOp.val);
            printf("source operand name: %s\n", actualInstruct.srcOp.macroName);
            printf("source operand index name: %s\n", actualInstruct.srcOp.indexName);
            printf("\ndestination operand type: %d\n", actualInstruct.destOp.type);
            printf("destination operand value: %d\n", actualInstruct.destOp.val);
            printf("destination operand name: %s\n", actualInstruct.destOp.macroName);
            printf("destination operand index name: %s\n", actualInstruct.destOp.indexName);
            
            mvToNextLine(&srcFile);
            actLineInSrc++;
            haveErrorInLine = 0;
        }
        
        else if (firstWordType == blank_line || firstWordType == in_comment) {
            mvToNextLine(&srcFile);
            actLineInSrc++;
        }
        
        else if (firstWordType == end_src_file) {
            endOfSrc = 1;
        }
    }
    
    freeSignTab(signTabHead);
    
    return 0;
}


