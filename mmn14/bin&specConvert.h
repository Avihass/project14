
#ifndef bin_specConvert_h
#define bin_specConvert_h

#define BIN_WORD_SIZE 14

typedef char binAdressWord[BIN_WORD_SIZE];

void resetBinWord(char* binWord);
void cpyBinWord(char* binDest, char* binSrc);

/* convert a decimal number to binary in a specific place in binary adress word */
void insrtDecToBin(char* binDest, int decimal, int begin, int limit);
#endif /* bin_specConvert_h */
