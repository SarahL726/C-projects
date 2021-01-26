#include <stdio.h>

int main () {
   FILE *fp;
   int c;
  
   fp = fopen("doc1.txt","r");
   if (fp == NULL){
       printf("It's NULL.");
   }   
   fclose(fp);
   
   return(0);
}
