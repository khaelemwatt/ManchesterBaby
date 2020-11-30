#include <stdio.h>
#include <stdlib.h>
 
int main()
{
   char ch;
   FILE *fp;
   char line[32];
   //printf("Enter the name of file:: ");
   //gets(file_name);
   fp = fopen("BabyTestAssembler.txt","r");
   if( fp == NULL ){
        printf("\nCan not open the file.");
        exit(0);
   }
   printf("\nThe contents of %s file is :\n", "BabyTestAssembler.txt");
   while(fgets(line, 32, fp) != NULL){
       if ()
       printf("%s", line);
   }
      
   fclose(fp);
 
   return 0;
}
