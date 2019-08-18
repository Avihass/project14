
#ifndef data_signTable_h
#define data_signTable_h

#include "bin&specConvert.h" /* for binAdressWord */

/* sign table */
typedef struct signTable* signTabPtr;
typedef struct signTable {
    
    char sign[MAX_MACRO_SIZE];
    int dataType;
    int value;
    signTabPtr next;
} signTab;

enum signType {
    
    macro_sign, code_sign, data_sign, external_sign
};

/* data table */
typedef struct dataTable* dataTabPtr;
typedef struct dataTable {
    
    int adress;
    binAdressWord binWord;
    dataTabPtr next;
} dataTab;

/* === sign table === */

void signTabCtor(signTabPtr* signTabHead); /* construct a first node in a signTabPtr */
void addSign(signTabPtr head, char* signStr, int type, int val);
int isAvailableSign(signTabPtr head, char* signStr); /* check the availablity of a
                                                    sign name */

int findMacro(signTabPtr head, char* macroName, int* findedVal);
void freeSignTab(signTabPtr head, signTabPtr tmp);

/* === data table === */

void dataTabCtor(dataTabPtr* dataTabHead);
void addData(dataTabPtr head, int ad, binAdressWord word);
void freeDataTab(dataTabPtr head, dataTabPtr tmp);
#endif /* data_signTable_h */
