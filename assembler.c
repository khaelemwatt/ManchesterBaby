#include <stdio.h>
#include <stdlib.h>
 
int main()
{
   FILE *fp;
   char line[256];
   fp = fopen("BabyTestAssembler.txt","r");
   if( fp == NULL ){
        printf("\nCan not open the file.");
        exit(0);
   }
   printf("\nThe contents of %s file is :\n", "BabyTestAssembler.txt");

    while(fgets(line, 256, fp) != NULL){
        char command[30];
        int index = 0;
        char ch = line[index];

       
        while (line[index]){
            if (line[index] == ';'){
                break;
            }
            else{
                printf("%c", line[index]);
            }
            index ++;
        }
        printf("\n");
   }
      
   fclose(fp);
 
   return 0;
}
