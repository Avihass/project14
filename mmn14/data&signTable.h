
#ifndef data_signTable_h
#define data_signTable_h

typedef struct signTable* signTabPtr;
typedef struct signTable {
    
    char sign[MAX_LINE];
    int dataType;
    int value;
    signTabPtr next;
} signTab;


void signTabCtor(signTabPtr* sign); /* construct a first node in a signTabPtr */
void addSign(signTabPtr* head, char* signStr, int type, int val);

#endif /* data_signTable_h */
