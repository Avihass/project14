
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"

int main(int argc, const char * argv[]) {
    
    char srcFileName[MAX_FILE_NAME + 3];
    FILE *srcFile;
    int readedFirstWord;
    int endOfSrc = 0;
    char macroName[MAX_LINE];
    int macroVal;
    signTabPtr signTabHead;
    
    if (strlen(argv[1]) >= MAX_FILE_NAME)
        printError("the source file name is to long");
    
    strcpy(srcFileName, argv[1]);
    strcat(srcFileName, ".as");
    
    if (!(srcFile = fopen("src.as", "r")))
        printError("can't open the source file");
    
    signTabCtor(&signTabHead);
    
    while (!endOfSrc) {
        
        readedFirstWord = readFirstWord(&srcFile);
        printf("the first word is: %d\n", readedFirstWord);
        
        if (readedFirstWord == in_macro) {
            
            macroVal = readMacro(&srcFile, macroName);
            addSign(signTabHead, macroName, macro_type, macroVal);
            
            printf("the name of the macro: %s\n", signTabHead->sign);
            printf("the value: %d\n", signTabHead->value);
        }
        
        endOfSrc = 1;
        freeSignTab(signTabHead);
    }
    
    return 0;
}

