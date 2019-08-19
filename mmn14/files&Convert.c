
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files&Convert.h"
#include "assembly.h"
#include "utils.h"

void resetBinWord(char* binWord) {
    
    int i;
    
    for (i = 0; i < BIN_WORD_SIZE; i++) {
        
        binWord[i] = 0;
    }
}

void cpyBinWord(char* binDest, char* binSrc) {
    
    int i;
    
    for (i = 0; i < BIN_WORD_SIZE; i++) {
        
        binDest[i] = binSrc[i];
    }
}

void insrtDecToBin(char* binDest, int decimal, int begin, int limit) {
    
    int i;
    
    for (i = begin; i <= limit && i < BIN_WORD_SIZE; i++) {
        
        binDest[i] = 01 & decimal;
        decimal = decimal >> 1;
    }
}

void binToSpec(char* binWord, char* specWord) {
    
    int binIndex = BIN_WORD_SIZE - 1, specIndex = 0;
    
    while (specIndex < SPEC_WORD_SIZE) {
        
        if (binWord[binIndex] == 0) {
            
            if (binWord[binIndex - 1] == 0)
                specWord[specIndex] = '*';
            
            /* the second bit is 1 */
            else
                specWord[specIndex] = '#';
        }
        
        /* the first bit is 1 */
        else {
            
            if (binWord[binIndex - 1] == 0)
                specWord[specIndex] = '%';
            
            else
                specWord[specIndex] = '!';
        }
        
        binIndex -= 2;
        specIndex++;
    }
}

void createObFile(char* srcName, int IC, int DC) {
    
    FILE* file;
    char newSrcName[MAX_FILE_NAME + 4];
    
    strcpy(newSrcName, srcName);
    strcat(newSrcName, ".ob");
    
    if (!(file = fopen(newSrcName, "a")))
        printErrorAndStop("can't open the object file");
    
    fprintf(file, "\t%d %d\n", IC - 100, DC);
    
    fclose(file);
}

void writeToObjectFile(char* srcName, int adress, char* specWord) {
    
    FILE* file;
    char newSrcName[MAX_FILE_NAME + 4];
    
    strcpy(newSrcName, srcName);
    strcat(newSrcName, ".ob");
    
    if (!(file = fopen(newSrcName, "a")))
        printErrorAndStop("can't open the object file");
    
    /* if true print 0 before the adress */
    if (adress < 1000)
        fprintf(file, "0%d\t%s\n", adress, specWord);
    
    else
        fprintf(file, "%d\t%s\n", adress, specWord);
    
    fclose(file);
}

void writeToEntFile(char* srcName, char* optCharName, int adress) {
    
    FILE* file;
    char newSrcName[MAX_FILE_NAME + 5];
    
    strcpy(newSrcName, srcName);
    strcat(newSrcName, ".ent");
    
    if (!(file = fopen(newSrcName, "a")))
        printErrorAndStop("can't open the entry file");
    
    /* if true print 0 before the adress */
    if (adress < 1000)
        fprintf(file, "%s\t0%d\n", optCharName, adress);
    
    else
        fprintf(file, "%s\t%d\n", optCharName, adress);
    
    fclose(file);
}

void writeToExtFile(char* srcName, char* optCharName, int adress) {
    
    FILE* file;
    char newSrcName[MAX_FILE_NAME + 5];
    
    strcpy(newSrcName, srcName);
    strcat(newSrcName, ".ext");
    
    if (!(file = fopen(newSrcName, "a")))
        printErrorAndStop("can't open the extern file");
    
    /* if true print 0 before the adress */
    if (adress < 1000)
        fprintf(file, "%s\t0%d\n", optCharName, adress);
    
    else
        fprintf(file, "%s\t%d\n", optCharName, adress);
    
    fclose(file);
}
