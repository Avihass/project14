
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"
#include "files&Convert.h"

/* ============ SIGN TABLE ============ */

/* constructor for signTab pointer, allocate a node with empty values */
void signTabCtor(signTabPtr* signTabHead) {
    
    signTabPtr signTmp;
    signTmp = (signTabPtr) malloc(sizeof(signTab));
    
    if (!signTmp)
        printErrorAndStop("can't allocate memory");
    
    /* initialize the sign name */
    strcpy(signTmp->sign, "");
    signTmp->next = NULL;
    
    *signTabHead = &(*signTmp);
}

void addSign(signTabPtr head, char* signStr, int type, int val) {
    
    /* if the sign table is empty */
    if (strcmp(head->sign, "") == 0) {
        
        strcpy(head->sign, signStr);
        head->dataType = type;
        head->value = val;
    }
    
    else {
        
        /* create new sign */
        signTabPtr signTmp;
        signTabPtr signIndex = head;
        signTmp = (signTabPtr) malloc(sizeof(signTab));
        
        if (!signTmp)
            printErrorAndStop("can't allocate memory");
        
        strcpy(signTmp->sign, signStr);
        signTmp->dataType = type;
        signTmp->value = val;
        signTmp->next = NULL;
        
        /* go to end of the table */
        while (signIndex->next != NULL) {
            signIndex = signIndex->next;
        }
        
        /* add the sign */
        signIndex->next = signTmp;
    }
}

int isAvailableSign(signTabPtr head, char* signName) {
    
    if (strcmp(head->sign, signName) == 0)
        return 0;
    
    if (head->next == NULL)
        return 1;
    
    return isAvailableSign(head->next, signName);
}

signTabPtr searchSign(signTabPtr head, char* signName) {
    
    if (strcmp(head->sign, signName) == 0) {
        
        return head;
    }
    
    if (head->next == NULL)
        return NULL;
    
    return searchSign(head->next, signName);
}

int findSign(signTabPtr head, char* signName, int signType, int* findedVal) {
    
    if (head->dataType == signType) {
        
        if (strcmp(head->sign, signName) == 0) {
            
            *findedVal = head->value;
            return 1;
        }
    }
    
    if (head->next == NULL)
        return 0;
    
    return findSign(head->next, signName, signType, findedVal);
}

void updateDataSign(signTabPtr head, int IC) {
    
    if (head->dataType == data_sign)
        head->value += IC;
    
    if (head->next == NULL)
        return;
    
    updateDataSign(head->next, IC);
}

int updateEntrySign(signTabPtr head, char* entryName) {
    
    if (strcmp(head->sign, entryName) == 0) {
        
        if (head->dataType == macro_sign || head->dataType == external_sign)
            return 0;
        
        else {
            
            head->dataType = entry_sign;
            return 1;
        }
    }
    
    if (head->next == NULL)
        return 0;
    
    return updateEntrySign(head->next, entryName);
}

void freeSignTab(signTabPtr head, signTabPtr tmp) {
    
    tmp = head->next;
    
    free(head);
    
    if (tmp == NULL)
        return;
        
    freeSignTab(tmp, NULL);
}

/* =========== DATA TABLE =========== */

/* constructor for dataTab pointer, allocate a node with empty values */
void dataTabCtor(dataTabPtr* dataTabHead) {
    
    dataTabPtr dataTmp;
    dataTmp = (dataTabPtr) malloc(sizeof(dataTab));
    
    if (!dataTmp)
        printErrorAndStop("can't allocate memory");
    
    /* initialize the sign name */
    dataTmp->adress = -1;
    resetBinWord(dataTmp->binWord);
    dataTmp->next = NULL;
    
    *dataTabHead = &(*dataTmp);
}

void addData(dataTabPtr head, int ad, binAdressWord word) {
    
    /* if the data table is empty */
    if (head->adress == -1) {
        
        head->adress = ad;
        cpyBinWord(head->binWord, word);
    }
    
    else {
        
        /* create new data node */
        dataTabPtr dataTmp;
        dataTabPtr dataIndex = head;
        dataTmp = (dataTabPtr) malloc(sizeof(dataTab));
        
        if (!dataTmp)
            printErrorAndStop("can't allocate memory");
        
        dataTmp->adress = ad;
        cpyBinWord(dataTmp->binWord, word);
        dataTmp->next = NULL;
        
        /* go to end of the table */
        while (dataIndex->next != NULL) {
            dataIndex = dataIndex->next;
        }
        
        /* add the sign */
        dataIndex->next = dataTmp;
    }
}

void freeDataTab(dataTabPtr head, dataTabPtr tmp) {
    
    tmp = head->next;
    
    free(head);
    
    if (tmp == NULL)
        return;
    
    freeDataTab(tmp, NULL);
}

