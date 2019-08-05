#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WORD_SIZE 14

typedef char binaryWord[14];

void unsignedBitNum(int, char*);
void printArr(char*);
void placeOpcode(char* field,int start, int limit, int num);


int main()
{
    binaryWord word = {0};
    int b=4;
    
    placeOpcode(word, 4, 7, b);
    
    printArr(word);
    /*unsignedBitNum(a,nArr);
     printArr(nArr);
     placeOpCode(nArr,9,11,a);
     printArr(nArr);*/
    return 0;
}

void placeOpcode(char* field,int start, int limit, int num){
    
    while(start<=limit){
        field[start]=01&num;
        num=num>>1;
        start++;
    }
}

void unsignedBitNum(int num, char* numArr)
{
    
    int i;
    for(i=WORD_SIZE-1; i>=0; i--)
    {
        numArr[i]=01&num;
        num=num>>1;
    }
}

void printArr(char* arrN)
{
    int i;
    for(i=0; i<WORD_SIZE; i++)
    {
        printf("%d\t", arrN[i]);
    }
    printf("\n");
}
