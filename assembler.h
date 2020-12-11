#ifndef __ASSEMBLER_HEADER__
#define __ASSEMBLER_HEADER__

// ### INCLUDE ### 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// ### DATA STRUCTURES ### 
typedef struct line {
	//Node of a linked list to store a line where a symbol is referenced
	int lineNum;
	struct line *next;
} Line;

typedef struct symbol {
	//Linked list to store symbols in the assembly code and to hold all lines it is referenced on
	int declaration;
	char name[50];
	Line *line;
	struct symbol *next;
} Symbol;

// ### INITIAL DECLARATIONS ### 
void symbolLink(int symbolDec, int lineNum);
void applySymbols();
Symbol* symbolExists(char* symbol);
char* stripWhiteSpace(char *s);
void addLine(char* symbol, int lineNum);
void checkFirst(char first[], int lineNumber);
int initialiseMC();
void printMC(int lines);
void convertFunc(char function[]);
char * convertInt(int num);
int checkOperand(char operand[], int LineNumber);
int checkCommand(char command[], int lineNumber);
FILE *filePicker();
int save(int LineNumber);
void parseComments(FILE *f);
int validFile(FILE *f);

#endif
