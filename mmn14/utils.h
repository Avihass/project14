
#ifndef utils_h
#define utils_h

extern int haveError;
extern int haveErrorInLine;

void printError(char* ErrorMessage); /* print the error message */
void printErrorInSrcFile(char* ErrorMessage); /* print error message and the line in the file */
void printErrorAndStop(char* ErrorMessage); /* print the error message and quit the programme */
#endif /* utils_h */
