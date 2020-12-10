#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
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

**/


/**
 * Adds the binary at the start of the line to link to the variable declaration at the bottom of the assembly code
 * */
void symbolLink(int symbolDec, int lineNum)
{
    char *binary = convertInt(symbolDec);

    for (int i=0; i<12; i++)
    {
        machineCode[lineNum][i] = binary[i];
    }
}

/**
 * Loops through all symbols and all lines the symbol is refferenced on, calling symbolLink for each.
 * */
void applySymbols()
{
    Symbol *currentS = head;
    while(currentS)
    {
        Line *currentLine = currentS->line;
        while(currentLine)
        {
            symbolLink(currentS->declaration, currentLine->lineNum);

            currentLine = currentLine->next;
        }

        currentS = currentS->next;
    }
}

/**
 * Function to check if a symbol exists within the symbol table
 * Returns
 *  NULL if symbol not found
 * Parameters
 *  char* symbol - name of the symbol to check for   
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

/**
 * strips a string of all white space characters
 * Returns
 *  char array without whitespace chars
 * Parameters
 *  char *s - char array to remove whitespace from 
 * */
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
 * Parameters:
 *  char* symbol - name of the symbol to add
 *  int lineNUm - the number the symbol is present on 
 * */
void addLine(char* symbol, int lineNum)
{
    Symbol *s; 
    Line *l;

    int exists = 1;

    symbol = stripWhiteSpace(symbol);

    //initialise line struct
    l = (Line*)malloc(sizeof(Line));
    l->lineNum = lineNum;

    //check if symbol exists 
    s = symbolExists(symbol);

    //if symbol doesnt exist, initialise a new sybol
    if (!s)
    {   
        exists = 0;
        s = (Symbol*)malloc(sizeof(Symbol));
    }

    //if symbol already contains a line 
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
lineNumber - the line the potentially useful symbol is on

this function checks the first command on the line in the assembly code and
uses it to see if it's useful. the only useful first commands will be the variables
which are stated at the end of the code.

**/
void checkFirst(char first[], int lineNumber){
    char *ptr = strstr(first, "VAR");

    if (ptr)
    {
        int exists = 1;

        int i=0;
        char currChar = first[0];

        while (currChar != ':')
        {
            i++;
            currChar = first[i];
        }
        char varName[i+1];

        for (int z=0; z<i; z++)
            varName[z] = first[z];

        varName[i] = 0;

        //printf("%s\n", varName);

        Symbol *s;

        s = symbolExists(varName);

        if (!s)
        {   
            exists = 0;
            s = (Symbol*)malloc(sizeof(Symbol));
        }

        s->declaration = lineNumber;

        strncpy(s->name, varName, 50);

        if (head && s != head && exists == 0)
        {
            s->next = head;
            head = s;
        } else if (!head)
            head = s;
    
    }
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
    printf("\n");
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

    int useful = 0;

    int i = 0;      
    while(i < 32){
        first[i] = command[i];
        
        if (command[i] == ':')
            useful = 1;

        i++;
    }
    
    if (useful == 1)
        checkFirst(first, lineNumber);
    
    int r = 10;
    i = 0;
    while(r < 13 || command[r] != ' ' ){
        function[i] = command[r];
        i++;
        r++;
    }

    r = 14;
    i = 0;
    while(r < 22 || command[r] != ' '){
        operand[i] = command[r];
        i++;
        r++;
    }
    
    checkOperand(operand, lineNumber);

    convertFunc(function);


    return 0;
}

/**
 * Used to prompt the user to select an assembly file
 * Returns 
 *  the file selected by the user 
 * */
FILE *filePicker()
{
    FILE *f;
    char filePath[FILENAME_MAX];

    printf("%s", "please enter the path to the assembly file:\n");
    scanf("%s", filePath);
    
    f = fopen(filePath, "r");
    while(!f)
    {
        printf("%s", "unable to open file, please input a correct file path. Type 'quit' to quit the program\n");
        scanf("%s", filePath);

        if (strncmp(filePath, "quit", FILENAME_MAX) == 0)
            exit(0);
        
        f = fopen(filePath, "r");
    }

    while(validFile(f) < 1)
    {
        printf("%s", "The file you selected does not appear to be valid assembly code, please select another. Type 'quit' to quit the program\n");
        scanf("%s", filePath);

        if (strncmp(filePath, "quit", FILENAME_MAX) == 0)
            exit(0);
        
        f = fopen(filePath, "r");
    }

    return f;
}

/**
 * used to save the machine code to a file
 * Returns: 0 if successful and 1 if not 
 * Parameters: 
 *  linenumber - number of lines present in the machinecode array
 * */
int save(int lineNumber)
{
    FILE *f;
    char filePath[FILENAME_MAX];

    printf("%s", "please enter the path to save the machine code to (including file name):\n");
    scanf("%s", filePath);

    while(1)
    {
        //check if file exists
        if( access( filePath, F_OK ) == 0 ) {
            //clear characters input by user 
            int c;
            while((c=getchar()) != EOF && c != '\n');

            char choice;

            printf("the file name you entered already exists, would you like to overwrite? [y/n]");
            scanf(" %c", &choice);
           

            if (choice == 'y')
            {   
                f = fopen(filePath, "w");
                if (!f)
                    return 1;

                break;

            } else if (choice == 'n')
            {
                printf("%s", "please enter the path to save the machine code to (including file name):\n");
                scanf("%s", filePath);
                continue;
            } else
            {
                choice = 0;
            }
            
        } else {
            f = fopen(filePath, "w");
            if (!f)
                return 1;
            break;
        }
    }

   
    for (int i = 0; i < lineNumber; i++){
        for (int j = 0; j < 32; j++){
            fprintf(f, "%c",machineCode[i][j]);
        }

        if (i != (lineNumber - 1))
            fprintf(f, "%c", '\n');
    }

    fclose(f);

    return 0;
}

/**
 * parses comments and starts the pipeline of breaking down the assembly code 
 * Parameters:
 *  FILE *f - file containing assembly code
 * */
void parseComments(FILE *f)
{
    char line[256];

    while(fgets(line, 256, f) != NULL){
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
            lineNumber ++;
        }
   }
}

/**
 * Function to detect if a selected file is assembly code
 * NOTE: does not detect syntax errors 
 * Returns:
 *  1 if valid
 *  0 if not valid
 * Parameters:
 *  FILE *f - file selected by user 
 * */
int validFile(FILE *f)
{
    char line[256];
    int i = 0;

    while(fgets(line, 256, f) != NULL){
        if (strstr(line, " JMP "))
            i++;
        else if (strstr(line, " JRP "))
            i++;
        else if (strstr(line, " LDN "))
            i++;
        else if (strstr(line, " STO "))
            i++;
        else if (strstr(line, " SUB "))
            i++;
        else if (strstr(line, " CMP "))
            i++;
        else if (strstr(line, " STP "))
            i++;
    }

    rewind(f);   

    return i;
}

// main
int main()
{
    FILE *fp;

    initialiseMC();

    fp = filePicker();
    parseComments(fp);
    fclose(fp);

    applySymbols();

    printMC(lineNumber);

    if (save(lineNumber) != 0)
    {
        printf("%s", "error saving file, please try again");
    }

   return 0;
}
