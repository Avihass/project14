#include <stdio.h>
#include <stdlib.h>
#define WORD_SIZE 14
void unsignedBitNum(int, char[]);


void unsignedBitNum(int num,char numArr[WORD_SIZE]){

int i,j,k,temp;
 temp=num;
for(i=WORD_SIZE;i>0;i--)
{
numArr[i]=01&num;
num=num>>1;
}//end for//
if(temp<0){
for(j=0;j<WORD_SIZE;j++){
num=numArr[j]^1;
num=num<<1;
}//end for//
num+=1;
for(k=WORD_SIZE;k>0;k++){
numArr[k]=01&num;
num=num>>1;
  }//end for//
 }//end if//
}//end program//

void main(){
int a,b,c;
char nArr[WORD_SIZE];
printf("select an unsigned number to dispalay \n");
scanf("%d",&c);
unsignedBitNum(c,nArr[WORD_SIZE]);
printf("the bit representation of the chosen number is %\n", nArr[WORD_SIZE]);

}
