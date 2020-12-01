#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "header.h"

Baby* createBaby(){
	//Allocates memory for the baby structure and initialises all values to 0
	Baby* pBaby = NULL;
    pBaby = (Baby*)malloc(sizeof(Baby));
    if(pBaby==NULL){
    	printf("MEMORY ALLOCATION ERROR");
    }

    for(int i=0; i<MEMSIZE; i++){
    	pBaby->accumulator[i] = '0';
    	pBaby->controlInstruction[i] = '0';
    	pBaby->presentInstruction[i] = '0';
    }
    for(int i=0; i<ADDRESSBUSSIZE; i++){
    	pBaby->addressBus[i] ='0';
    }
    for(int i=0; i<CONTROLBUSSIZE; i++){
    	pBaby->controlBus[i] = '0';
    }
    for(int i=0; i<NUMBEROFADDRESSES; i++){
    	for(int j=0; j<MEMSIZE; j++){
    		pBaby->store[i][j] = '0';
    	}
    }

    return pBaby;
}

int binToDec(char bin[], int length){
	//Takes an array of chars (binary string) and returns the decimal value
	int decimal = 0;
	for(int i=0; i<length; i++){
		if(bin[i]=='1'){
			decimal += (int)pow(2, i);
		}
	}

	return decimal;
}

void decToBin(int dec, char bin[]){
	//Takes a decimal and a char array for the values to be copied into and returns the binary string
	int remainder;
	int i=0;
	while(dec>0){
		remainder = dec % 2;
		dec = dec / 2;

		bin[i] = remainder + '0';

		i++;
	}
}

void printBin(char bin[], int length){
	//For easy printing of binary numbers (mostly for debugging purposes)
    for(int i=0; i<length; i++){
		printf("%c", bin[i]);
	}
	printf("\n");
}

void incrementCI(Baby* baby){
	int count = binToDec(baby->controlInstruction, MEMSIZE);
	count += 1;
	decToBin(count, baby->controlInstruction);

}

void fetch(){
	//Get the instruction from the data store
}

void decode(){
	//Get the memory address of the operand and OP code from the current instruction
}

void execute(){
	//Carry out the instruction
}

void displayBaby(){
	//Display the contents of the data store
}

int main(){ //Main loop for the fetch decode execute cycle

    Baby* baby = NULL;
    baby = createBaby();

    printBin(baby->controlInstruction, MEMSIZE);
    printf("%d\n", binToDec(baby->controlInstruction, MEMSIZE));

	incrementCI(baby);

	printf("Control Instruction incremented\n");
	printBin(baby->controlInstruction, MEMSIZE);
    printf("%d\n", binToDec(baby->controlInstruction, MEMSIZE));


	fetch(baby);
	decode(baby);
	execute(baby);
	displayBaby(baby);

}
