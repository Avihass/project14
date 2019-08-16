
#include <stdio.h>
#include <stdlib.h>
#include "bin&specConvert.h"

void resetBinWord(char* binWord) {
    
    int i;
    
    for (i = 0; i < BIN_AD_LENGTH; i++) {
        
        binWord[i] = 0;
    }
}
