
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readSource.h"
#include "data&signTable.h"
#include "utils.h"

void signTabCtor(signTabPtr* sign) {
    
    signTabPtr signTmp;
    signTmp = (signTabPtr) malloc(sizeof(signTab));
    
    if (!signTmp)
        printError("can't allocate memory");
    
    /* initialize the sign name */
    strcpy(signTmp->sign, "");
    
    *sign = &(*signTmp);
}

void addSign(signTabPtr head, char* signStr, int type, int val) {
    
    /* if the sign table is empty */
    if (strcmp(head->sign, "")) {
        
        strcpy(head->sign, signStr);
        head->dataType = type;
        head->value = val;
    }
    
    else {
        
        /* create new sign */
        signTabPtr signTmp;
        signTabPtr signIndex = head->next;
        signTmp = (signTabPtr) malloc(sizeof(signTab));
        
        if (!signTmp)
            printError("can't allocate memory");
        
        strcpy(signTmp->sign, signStr);
        signTmp->dataType = type;
        signTmp->value = val;
        
        /* go to end of the table */
        while (signIndex != NULL) {
            signIndex = signIndex->next;
        }
        
        /* add the sign */
        signIndex->next = &(*signTmp);
    }
}

int isAvailable(signTabPtr head, char* signStr) {
    
    if (strcmp(head->sign, signStr))
        return 0;
    
    if (head->next == NULL)
        return 1;
    
    return isAvailable(head->next, signStr);
}
