
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"
#include "files&Convert.h"

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
            if (!findSign(signTabHead, actualInstruct.OP.macroName, macro_sign, &tmpNum)) \
                printErrorInSrcFile("used undeclared macro name as imediate value"); \
        } \
    } \
 \
    else if(actualInstruct.OP.type == index_met) { \
 \
        if (strcmp(actualInstruct.OP.indexName, "") != 0) { \
\
            if (!findSign(signTabHead, actualInstruct.OP.indexName, macro_sign, &tmpNum)) \
                printErrorInSrcFile("used undeclared macro name in index"); \
        } \
    } \
}

/* the actual line in source file number */
int actLineInSrc = 1;

int main(int argc, const char * argv[]) {
    
    char srcFileName[MAX_FILE_NAME + 4] = {'\0'}; /* +4 character for ".as" and '\0' */
    int argIndx = 1;
    FILE *srcFile = NULL;
    int firstWordType;
    char readedFirstWord[MAX_MACRO_SIZE] = {'\0'};
    int endOfSrc = 0;
    char macroName[MAX_MACRO_SIZE] = {'\0'};
    int macroVal;
    char optCharName[MAX_MACRO_SIZE] = {'\0'};
    int haveOptChar = 0;
    signTabPtr signTabHead;
    dataTabPtr dataTabHead;
    dataTabPtr codeTabHead;
    instructField actualInstruct;
    binAdressWord actualAdWord;
    int IC, DC;
    
    /* construct the tables */
    signTabCtor(&signTabHead);
    dataTabCtor(&dataTabHead);
    dataTabCtor(&codeTabHead);
    
    if (argc < 2)
        printError("missing file name in argument");
    
    if (!haveError && argIndx < argc) {
        
        if (strlen(argv[argIndx]) >= MAX_FILE_NAME)
            printError("the source file name is to long");
        
        if (!haveError) {
            
            strcpy(srcFileName, argv[argIndx]);
            strcat(srcFileName, ".as");
            
            if (!(srcFile = fopen(srcFileName, "r")))
                printErrorAndStop("can't open the source file");
            
            else
                printf("==%s opened==\n\n", srcFileName);
        }
        
        IC = 100;
        DC = 0;
        
        /* ===== FIRST PASSAGE ===== */
        
        if (!haveError) {
            
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
                    char macroName[MAX_MACRO_SIZE] = {'\0'};
                    
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
                            
                            if (!findSign(signTabHead, macroName, macro_sign, &dataNum))
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
                            
                            addSign(signTabHead, optCharName, code_sign, IC);
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
                
                /* firstWordType is -1 */
                else {
                    
                    GO_NEXT_LINE
                }
            }/* end of first passage */
        }
        
        /* ===== SECONDE PASSAGE ===== */
        
        if (!haveError) {
            
            updateDataSign(signTabHead, IC);
            endOfSrc = 0;
            actLineInSrc = 1;
            IC = 100;
            
            /* go back to the file begining */
            fseek(srcFile, 0, SEEK_SET);
        }
        
        while (!endOfSrc && !haveError) {
            
            firstWordType = readFirstWord(srcFile, readedFirstWord);
            
            if (firstWordType == optional_char)
                firstWordType = readFirstWord(srcFile, readedFirstWord);
            
            /* if true ignore the line */
            if (firstWordType != entry_line && firstWordType != instruction_line &&
                firstWordType != end_src_file) {
                
                mvToNextLine(srcFile);
                actLineInSrc++;
            }
            
            else if (firstWordType == entry_line) {
                
                char readedEntry[MAX_MACRO_SIZE];
                
                readEntryOrExtern(srcFile, readedEntry);
                
                if (!updateEntrySign(signTabHead, readedEntry) && !haveError)
                    printErrorInSrcFile("the optional charactere to entry is not declared or is a macro");
                
                mvToNextLine(srcFile);
                actLineInSrc++;
            }
            
            else if (firstWordType == instruction_line) {
                
                signTabPtr foundedSign = NULL;
                int haveRegInSrc = 0;
                
                actualInstruct.type = identifyInstruction(readedFirstWord);
                actualInstruct = readInstruction(srcFile, actualInstruct.type);
                resetBinWord(actualAdWord);
                
                /* convert to binary adress word */
                insrtDecToBin(actualAdWord, actualInstruct.type, 6, 9);
                
                if (actualInstruct.srcOp.type != -1)
                    insrtDecToBin(actualAdWord, actualInstruct.srcOp.type, 4, 5);
                
                if (actualInstruct.destOp.type != -1)
                    insrtDecToBin(actualAdWord, actualInstruct.destOp.type, 2, 3);
                
                /* set ARE to 00 */
                insrtDecToBin(actualAdWord, 0, 0, 1);
                
                /* add the binary word with adress */
                addData(codeTabHead, IC, actualAdWord);
                IC++;
                
                /* if true, convert the source and add (1 or 2 binary word) to the code table */
                if (actualInstruct.srcOp.type != -1) {
                    
                    resetBinWord(actualAdWord);
                    
                    if (actualInstruct.srcOp.type == imediate_met) {
                        
                        /* if true the operand is a macro */
                        if (strcmp(actualInstruct.srcOp.macroName, "") != 0)
                            findSign(signTabHead, actualInstruct.srcOp.macroName, macro_sign, &(actualInstruct.srcOp.val));
                        
                        insrtDecToBin(actualAdWord, actualInstruct.srcOp.val, 2, 13);
                            
                        /* set ARE to 00 */
                        insrtDecToBin(actualAdWord, 0, 0, 1);
                        
                        addData(codeTabHead, IC, actualAdWord);
                        IC++;
                    }
                    
                    else if (actualInstruct.srcOp.type == direct_met || actualInstruct.srcOp.type == index_met) {
                        
                        foundedSign = searchSign(signTabHead, actualInstruct.srcOp.macroName);
                        
                        if (foundedSign) {
                            
                            if (foundedSign->dataType != macro_sign) {
                                
                                insrtDecToBin(actualAdWord, foundedSign->value, 2, 13);
                                
                                if (foundedSign->dataType == external_sign)
                                    insrtDecToBin(actualAdWord, 1, 0, 1);
                                
                                else
                                    insrtDecToBin(actualAdWord, 2, 0, 1);
                                
                                addData(codeTabHead, IC, actualAdWord);
                                IC++;
                                
                                if (actualInstruct.srcOp.type == index_met) {
                                    
                                    resetBinWord(actualAdWord);
                                    insrtDecToBin(actualAdWord, actualInstruct.srcOp.val, 2, 13);
                                    insrtDecToBin(actualAdWord, 0, 0, 1);
                                    addData(codeTabHead, IC, actualAdWord);
                                    IC++;
                                }
                            }
                            
                            else
                                printErrorInSrcFile("used macro as direct operand method");
                        }
                        
                        else
                            printErrorInSrcFile("used undeclared optional character");
                    }
                    
                    /* the operand method is register */
                    else {
                        
                        haveRegInSrc = 1;
                        
                        insrtDecToBin(actualAdWord, actualInstruct.srcOp.val, 5, 7);
                        
                        /* if the destination operand is a register too, use only 1 word */
                        if (actualInstruct.destOp.type == register_met)
                            insrtDecToBin(actualAdWord, actualInstruct.destOp.val, 2, 4);
                        
                        insrtDecToBin(actualAdWord, 0, 0, 1);
                        addData(codeTabHead, IC, actualAdWord);
                        IC++;
                    }
                }
                
                if (actualInstruct.destOp.type != -1) {
                    
                    resetBinWord(actualAdWord);
                    
                    if (actualInstruct.destOp.type == imediate_met) {
                        
                        /* if true the operand is a macro */
                        if (strcmp(actualInstruct.srcOp.macroName, "") != 0)
                            findSign(signTabHead, actualInstruct.srcOp.macroName, macro_sign, &(actualInstruct.srcOp.val));
                        
                        insrtDecToBin(actualAdWord, actualInstruct.srcOp.val, 2, 13);
                        
                        /* set ARE to 00 */
                        insrtDecToBin(actualAdWord, 0, 0, 1);
                        
                        addData(codeTabHead, IC, actualAdWord);
                        IC++;
                    }
                    
                    else if (actualInstruct.destOp.type == direct_met || actualInstruct.destOp.type == index_met) {
                        
                        foundedSign = searchSign(signTabHead, actualInstruct.destOp.macroName);
                        
                        if (foundedSign) {
                            
                            if (foundedSign->dataType != macro_sign) {
                                
                                insrtDecToBin(actualAdWord, foundedSign->value, 2, 13);
                                
                                if (foundedSign->dataType == external_sign) {
                                    
                                    insrtDecToBin(actualAdWord, 1, 0, 1);
                                    
                                    /* here we add the founded sign in the sign table as a "request" to add the sign
                                     in .ext file later with IC value */
                                    addSign(signTabHead, foundedSign->sign, external_sign, IC);
                                }
                                
                                else
                                    insrtDecToBin(actualAdWord, 2, 0, 1);
                                
                                addData(codeTabHead, IC, actualAdWord);
                                IC++;
                                
                                if (actualInstruct.destOp.type == index_met) {
                                    
                                    resetBinWord(actualAdWord);
                                    insrtDecToBin(actualAdWord, actualInstruct.destOp.val, 2, 13);
                                    insrtDecToBin(actualAdWord, 0, 0, 1);
                                    addData(codeTabHead, IC, actualAdWord);
                                    IC++;
                                }
                            }
                            
                            else
                                printErrorInSrcFile("used macro as direct operand method");
                        }
                        
                        else
                            printErrorInSrcFile("used undeclared optional character");
                    }
                    
                    /* the operand is register so we check if we allready add the register
                     with the source register */
                    else if (!haveRegInSrc) {
                        
                        insrtDecToBin(actualAdWord, actualInstruct.destOp.val, 2, 4);
                        insrtDecToBin(actualAdWord, 0, 0, 1);
                        addData(codeTabHead, IC, actualAdWord);
                        IC++;
                    }
                }
                
                mvToNextLine(srcFile);
                actLineInSrc++;
            }
            
            else if (firstWordType == end_src_file)
                endOfSrc = 1;
        } /* end of second passage */
        
        /* ===== CREATE FILES ===== */
        
        if (!haveError) {
            
            signTabPtr signIndx = signTabHead;
            
            /* remove ".as" */
            strcpy(srcFileName, argv[argIndx]);
            
            /* create .ent file */
            while (signIndx != NULL) {
                
                if (signIndx->dataType == entry_sign) {
                    
                    writeToEntFile(srcFileName, signIndx->sign, signIndx->value);
                }
                
                signIndx = signIndx->next;
            }
            
            /* create .ext file */
            signIndx = signTabHead;
            
            while (signIndx != NULL) {
                
                /* write the "requested" nodes there are external and with IC value
                   the 0 value is the original extern sign and not a request */
                if (signIndx->dataType == external_sign && signIndx->value != 0) {
                    
                    writeToExtFile(srcFileName, signIndx->sign, signIndx->value);
                }
                
                signIndx = signIndx->next;
            }
            
            
        }
        
        freeSignTab(signTabHead, NULL);
        freeDataTab(dataTabHead, NULL);
        freeDataTab(codeTabHead, NULL);
    }
    
    return 0;
}
