#ifndef __MANCHESTER_BABY_HEADER__ // this is a 'guard' on the header file, to prevent it's contents from
#define __MANCHESTER_BABY_HEADER__ // being redefined more than once during the build process

// ### DATA STRUCTURES ### 
#define MEMSIZE 32
#define NUMBEROFADDRESSES 32
#define ADDRESSBUSSIZE 5
#define CONTROLBUSSIZE 3

typedef struct baby {
	char accumulator[MEMSIZE];
	char controlInstruction[MEMSIZE];
	char presentInstruction[MEMSIZE];
	char store[NUMBEROFADDRESSES][MEMSIZE];
	char addressBus[ADDRESSBUSSIZE];
	char controlBus[CONTROLBUSSIZE];
}Baby;

Baby* createBaby();
int binToDec(char bin[], int length);
void decToBin(int dec, char bin[]);
void printBin(char bin[], int length);
void incrementCI();
int countChars(char filename[]);
int countRows(char filename[]);
Baby* loadStore();
Baby* fetch();
void decode();
void execute();
void displayBaby();

#endif
