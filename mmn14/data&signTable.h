
#ifndef data_signTable_h
#define data_signTable_h

typedef struct signTable* signTabPtr;
typedef struct signTable {
    
    char sign[MAX_LINE];
    int dataType;
    int value;
} signTab;

#endif /* data_signTable_h */
