
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"
#include "bin&specConvert.h"

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

int isAvailableSign(signTabPtr head, char* signStr) {
    
    if (strcmp(head->sign, signStr) == 0)
        return 0;
    
    if (head->next == NULL)
        return 1;
    
    return isAvailableSign(head->next, signStr);
}

void freeSignTab(signTabPtr head, signTabPtr tmp) {
    
    tmp = head->next;
    
    free(head);
    
    if (tmp == NULL)
        return;
        
    return freeSignTab(tmp, NULL);
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
    
    return freeDataTab(tmp, NULL);
}

