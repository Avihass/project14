
#ifndef readSource_h
#define readSource_h

#define MAX_LINE_SIZE 80
#define MAX_MACRO_SIZE 31

enum firstWordOpt {
  
    in_macro, data_line, string_line, entry_line, extern_line, optional_char, instruction_line, in_comment, end_src_file,
    blank_line
};

enum adressMethods {
    
    /* the value here is important */
    imediate_met = 0, direct_met, index_met, register_met
};

enum instructionType {
    
    /* the value here is important */
    inst_mov = 0, inst_cmp, inst_add, inst_sub, inst_not, inst_clr,
    inst_lea, inst_inc, inst_dec, inst_jmp, inst_bne, inst_red,
    inst_prn, inst_jsr, inst_rts, inst_stop
};

typedef struct AdressOperand {
    
    int type;
    int val;
    char macroName[MAX_MACRO_SIZE];
    char indexName[MAX_MACRO_SIZE];
} adOperand;

typedef struct instructionField {
    
    int type;
    adOperand srcOp;
    adOperand destOp;
} instructField;

int readFirstWord(FILE* file, char* readedWord);
int readMacro(FILE* file, char* macroName);
void mvToNextLine(FILE* file);
instructField readInstruction(FILE* file, int instructType);

/* return the instruction type and return -1 if the word
   is not an instruction */
int identifyInstruction(char* word);

int readDataDirective(FILE* file, char* macroName, int* isEnd);
void readStringDirective(FILE* file, char* strDest);
void readEntryOrExtern(FILE* file, char* optCharName);

#endif /* readSource_h */
