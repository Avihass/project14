
#include <stdio.h>
#include <stdlib.h>
#include "bin&specConvert.h"

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
    
    for (i = begin; i < limit && i < BIN_WORD_SIZE; i++) {
        
        binDest[i] = 01 & decimal;
        decimal = decimal >> 1;
    }
}
