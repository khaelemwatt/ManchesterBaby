#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

// global variables:
//char variables[16][16];   // this is a 2D array which contains all the variables referenced throughout the file
                          // you may want to use a different approach but this is what I tried

char machineCode[32][32]; // this is the 2D array which will be exported to the external file
int variableNum = 0;    // this is the number which 
int lineNumber = 0; // the current useable line of the assembly language

Symbol *head; //head of symbol table 

/**
assembler.c

This file takes in the assembly code language from an external file and converts it to machine code

So far: it can convert most of the necessary code to machine code.

To do:  *get the program to be able to detect the variables at the bottom of the assembly language 
        and assign the line number of the variables to the start of the lines which make use of them

        *export the machine code to an external file.

        *allow the user to manually enter the name of the assembly language text file

        *input validation
**/

/**
 * Function to check if the smbol exists within the symbol table
 * if it does exist it returns a pointer to the symbol
 * if id does not exist it returns null
 * 
 * */
Symbol* symbolExists(char* symbol)
{
    if (!head)
        return NULL;

    Symbol *current = head;
    while (current)
    {
        if (strncmp(symbol, current->name, 50) == 0)
            return current;
        
        current = current->next;
    }

    return NULL;
}

char* stripWhiteSpace(char *s)
{
    for (int i=0; i<strlen(s); i++)
    {
        if (s[i] == ' ')
            s[i] = 0;
    }
    
    return s;
}

/**
 * function to add a line to the symbol table 
 * if the symbol does not exist it also creates the symbol
 * 
 * */
void addLine(char* symbol, int lineNum)
{
    Symbol *s; 
    Line *l;
    
    symbol = stripWhiteSpace(symbol);

    //initialise line struct
    l = (Line*)malloc(sizeof(Line));
    l->lineNum = lineNum;

    //check if symbol exists 
    s = symbolExists(symbol);

    if (!s)
        s = (Symbol*)malloc(sizeof(Symbol));
    
    if (s->line)
    {
        l->next = s->line;
        s->line = l;
    } else
        s->line = l;
    
    strncpy(s->name, symbol, 50);

    if (head && s != head)
    {
        s->next = head;
        head = s;
    } else if (!head)
        head = s;
    
}

/**
checkFirst(char first[])
parameters:
first[]: first is the first command on the line in the assembly code.

this function checks the first command on the line in the assembly code and
uses it to see if it's useful. the only useful first commands will be the variables
which are stated at the end of the code.

**/

int checkFirst(char first[]){

}

/**
initialiseMC()

this function makes the machine code 2D array full of zeroes initially

**/
int initialiseMC(){

    for (int i = 0; i < 32; i++){
        for (int j = 0; j < 32; j++){

            machineCode[i][j] = '0';
        }
    }
    return 0;
}

/**
printMC(int lines)
parameters:
lines: the number of lines of the machine code you wish to print.

this function prints the machine code 2D array

**/
int printMC(int lines){
    for (int i = 0; i < lines; i++){
        for (int j = 0; j < 32; j++){
            printf("%c",machineCode[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

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
        machineCode[lineNumber][13] = '0';
        machineCode[lineNumber][14] = '0';
        machineCode[lineNumber][15] = '0';
    }
    else if (strcmp("JRP", function) == 0){
        machineCode[lineNumber][13] = '1';
        machineCode[lineNumber][14] = '0';
        machineCode[lineNumber][15] = '0';
    }
    else if (strcmp("LDN", function) == 0){
        machineCode[lineNumber][13] = '0';
        machineCode[lineNumber][14] = '1';
        machineCode[lineNumber][15] = '0';
    }
    else if (strcmp("STO", function) == 0){
        machineCode[lineNumber][13] = '1';
        machineCode[lineNumber][14] = '1';
        machineCode[lineNumber][15] = '0';
    }
    else if (strcmp("SUB", function) == 0){
        machineCode[lineNumber][13] = '0';
        machineCode[lineNumber][14] = '0';
        machineCode[lineNumber][15] = '1';
    }
    else if (strcmp("CMP", function) == 0){
        machineCode[lineNumber][13] = '0';
        machineCode[lineNumber][14] = '1';
        machineCode[lineNumber][15] = '1';
    }
    else if (strcmp("STP", function) == 0){
        machineCode[lineNumber][13] = '1';
        machineCode[lineNumber][14] = '1';
        machineCode[lineNumber][15] = '1';
    }
    else if (strcmp("VAR", function) == 0){
        machineCode[lineNumber][13] = '0';
        machineCode[lineNumber][14] = '0';
        machineCode[lineNumber][15] = '0';

        
    }
    else{
        printf("UNKNOWN FUNCTION ENTERED!");
    }
}

/**convertInt(int num)
parameters:
num: the number that needs to be converted to binary

this function takes in a number and converts it to binary in reverse for
outputting to the machine code file, as this requires binary to be in reverse
order.
**/
char * convertInt(int num){
    char * binaryNum = malloc(sizeof(13));
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
    return binaryNum;
}

/**
checkOperand(char operand[])
parameters:
operand[]: operand is the command on the line which the function is being performed on.

this function checks the operand to see if it is a variable or a number. if it is a number
it converts it to binary and adds it to the machine code array. This function returns 1 if the
operand is a variable.

**/
int checkOperand(char operand[], int lineNumber){
    int isVariable = 0;
    for (int i = 0; i < strlen(operand); i++){
        if (isalpha(operand[i])) {
            isVariable = 1;
            break;
        } 
    }
    if (isVariable == 1){
        // for (int r = 0; r < strlen(operand); r++){
        //     //variables[variableNum][r] = operand[r];
        //     addLine(operand, 0);
        // }
        //variableNum ++;
        addLine(operand, lineNumber);

        return 1;
    }
    else{
        int decimal;
        sscanf(operand, "%d", &decimal);
        char * binaryNum = convertInt(decimal);
        for (int i = 0; i < 13; i++){
           machineCode[lineNumber][i] = binaryNum[i];
        }
        return 0;
    }

}



/**checkCommand(char command[])
parameters:
command[]: the line of useable code taken from the assembly file

this function takes a useable line of code from the assembly file
and breaks it up into the appropriate sections for use.
**/
int checkCommand(char command[], int lineNumber)  
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

   //printf("%s", function);

    r = 14;
    i = 0;
    while(r < 22 || command[r] != ' '){
        operand[i] = command[r];
        i++;
        r++;
    }
    //printf("%s", operand);
    
    checkOperand(operand, lineNumber);

    convertFunc(function);
    //checkFirst(first);
    return 0;

}

// main
int main()
{
    FILE *fp;
    char line[256];
    initialiseMC();

 //  printMC(16);
    fp = fopen("BabyTestAssembler.txt","r");
    if( fp == NULL ){
        printf("\nCan not open the file.");
        exit(0);
    }

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
            checkCommand(command, lineNumber);
            //printf(" %d \n", lineNumber);
            lineNumber ++;
        }
        
   }
    
    Symbol *current = head;
    while (current)
    {
        printf("\n-----------%s----------\n", current->name);

        Line *line = current->line;
        while (line)
        {
            printf("- %d\n", line->lineNum);

            line = line->next;
        }

        current = current->next;
    }

    fclose(fp);
   //printMC(lineNumber);

   return 0;
}
