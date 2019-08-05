
#ifndef readSource_h
#define readSource_h

#define MAX_LINE 80

enum firstWordOpt {
  
    in_macro, entry_line, extern_line, optional_char, instruction_line, in_comment, end_src_file, blank_line
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
    char macroName[MAX_LINE];
} adOperand;

typedef struct instructionField {
    
    char name[MAX_LINE];
    int type;
    adOperand srcOp;
    adOperand destOp;
    int ARE;
} instructField;

int readFirstWord(FILE** file, char* readedWord);
int readMacro(FILE** file, char* macroName);
void mvToNextLine(FILE** file);
instructField readInstruction(FILE** file, char* instructName, int instructType);
int identifyInstruction(char* word); /* return the instruction type and return -1 if the word
                                        is not an instruction */

#endif /* readSource_h */
