#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/**
int checkFirst(char first[]){

    printf("%s", first);
    if (first[0] == 'S'){
        printf("start!");
    }
    return 0;
}
**/


/**
convertFunc(char function[])
parameters:
function[]: function is the command in the assembler file that performs the action
            e.g. JMP, LDN, SUB etc.

this function converts these functions and converts them to the appropriate 3 digit 
binary number for outputting to file.
**/
int convertFunc(char function[]){
    char funcBinary[4];
    if (strcmp("JMP", function) == 0){
        strcpy(funcBinary, "000");
        printf("%s", funcBinary);
    }
    else if (strcmp("JRP", function) == 0){
        strcpy(funcBinary, "100");
        printf("%s", funcBinary);
    }
    else if (strcmp("LDN", function) == 0){
        strcpy(funcBinary, "010");
        printf("%s", funcBinary);
    }
    else if (strcmp("STO", function) == 0){
        strcpy(funcBinary, "110");
        printf("%s", funcBinary);
    }
    else if (strcmp("SUB", function) == 0){
        strcpy(funcBinary, "001");
        printf("%s", funcBinary);
    }
    else if (strcmp("CMP", function) == 0){
        strcpy(funcBinary, "011");
        printf("%s", funcBinary);
    }
    else if (strcmp("STP", function) == 0){
        strcpy(funcBinary, "111");
        printf("%s", funcBinary);
    }
    else if (strcmp("VAR", function) == 0){
        printf("thats a var!");
    }
    else{
        printf("UNKNOWN FUNCTION ENTERED!");
    }
}


int checkOperand(char operand[]){

}

/**convertFunc(int num)
parameters:
num: the number that needs to be converted to binary

this function takes in a number and converts it to binary in reverse for
outputting to the machine code file, as this requires binary to be in reverse
order.
**/
int convertInt(int num){
    char binaryNum[13];
    int subtraction = 4096;
    int index = 12;
    while (index >= 0){
        if ((num - subtraction) >= 0){
            num = num - subtraction;
            binaryNum[index] = '1';
        }
        else{
            binaryNum[index] = '0';
        }
            subtraction = subtraction/2;
            index --;
    }
    printf("%s", binaryNum);
    return 0;
}

/**checkCommand(char command[])
parameters:
command[]: the line of useable code taken from the assembly file

this function takes a useable line of code from the assembly file
and breaks it up into the appropriate sections for use.
**/
int checkCommand(char command[])  
{
    char first[32];
    char operand[8];
    char function[32];

    int i = 0;      
    while(i < 6){
        first[i] = command[i];
        i++;
    }
    
    int r = 10;
    i = 0;
    while(r < 13 || command[r] != ' ' ){
        function[i] = command[r];
        i++;
        r++;
    }

   printf("%s", function);

    r = 14;
    i = 0;
    while(r < 22 || command[r] != ' '){
        operand[i] = command[r];
        i++;
        r++;
    }
    //printf("%s", operand);
    //convertFunc(function);
    //checkFirst(first);
    return 0;

}






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
        int validLine = 0;

       
        while (line[index]){
            char ch = line[index];
            
            if (line[index] == ';'){
                break;
            }
            else{
                command[index] = ch;
                validLine = 1;
            }
            index ++;
        }
        if (validLine == 1){
            checkCommand(command);
            printf("\n");
        }
        
   }
      
   fclose(fp);
 
   return 0;
}
