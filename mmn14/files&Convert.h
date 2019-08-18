
#ifndef bin_specConvert_h
#define bin_specConvert_h

#define BIN_WORD_SIZE 14
#define SPEC_WORD_SIZE 7

typedef char binAdressWord[BIN_WORD_SIZE];
typedef char specAdressWord[BIN_WORD_SIZE];

void resetBinWord(char* binWord);
void cpyBinWord(char* binDest, char* binSrc);

/* convert a decimal number to binary in a specific place in binary adress word */
void insrtDecToBin(char* binDest, int decimal, int begin, int limit);

/* write the value of IC and DC at the top of the file */
void createObFile(char* srcName, int IC, int DC);

/* create the file if not existe and write the data of arguments in the file */
void writeToObjectFile(char* srcName, int adress, char* specWord);
void writeToEntFile(char* srcName, char* optCharName, int value);
void writeToExtFile(char* srcName, char* optCharName, int value);
#endif /* bin_specConvert_h */
