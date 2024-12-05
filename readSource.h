
#ifndef readSource_h
#define readSource_h

#define MAX_LINE 80

enum firstWordOpt {

    in_macro, entry_line, extern_line, optional_char, instruction_line, in_comment
};

int readFirstWord(FILE** file);

#endif /* readSource_h */
