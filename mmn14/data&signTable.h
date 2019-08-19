
#ifndef data_signTable_h
#define data_signTable_h

#include "files&Convert.h" /* for binAdressWord */

/* sign table */
typedef struct signTable* signTabPtr;
typedef struct signTable {
    
    char sign[MAX_MACRO_SIZE];
    int dataType;
    int value;
    signTabPtr next;
} signTab;

enum signType {
    
    macro_sign, code_sign, data_sign, external_sign, entry_sign
};

/* data table */
typedef struct dataTable* dataTabPtr;
typedef struct dataTable {
    
    int adress;
    binAdressWord binWord;
    dataTabPtr next;
} dataTab;

/* === sign table === */

/* construct a first node in a signTabPtr */
void signTabCtor(signTabPtr* signTabHead);

void addSign(signTabPtr head, char* signStr, int type, int val);

/* check the availablity of a sign name */
int isAvailableSign(signTabPtr head, char* signName);

/* receive a sign name and return a poiter to the founded sign,
   if the sign is not founded the function return NULL */
signTabPtr searchSign(signTabPtr head, char* signName);

/* find a sign with there type, return 0 if not finded and if finded insert the sign
   value to findedVal */
int findSign(signTabPtr head, char* signName, int signType, int* findedVal);

/* update a sign to be internal, return 0 if the sign not found or if the
   sign is a macro */
int updateEntrySign(signTabPtr head, char* entryName);

/* update the adress of all data sign to there value + IC */
void updateDataSign(signTabPtr head, int IC);
void freeSignTab(signTabPtr head, signTabPtr tmp);

/* === data table === */

/* construct a first node in a dataTabPtr */
void dataTabCtor(dataTabPtr* dataTabHead);

void addData(dataTabPtr head, int ad, binAdressWord word);
void freeDataTab(dataTabPtr head, dataTabPtr tmp);
#endif /* data_signTable_h */
