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

//counts the nu,ber of lines in a file
int countRows(char filename[]){
  
    int rows =0;
    char ch;
    char last;

    FILE *fp;
    fp = fopen(filename, "r");

    if (fp != NULL){

        //Add initial line
        rows++;
        
        while ((ch = fgetc(fp)) != EOF)
        {
            /* Check new line */
            if (ch == '\n' || ch == '\0')
                rows++;
            last = ch;
        }

        if(last=='\n')
            rows--;

        fclose(fp);
    }
    return rows;

}

//count the number of chars in a file
int countChars(char filename[]){
    int chars =0;
    char ch;

    FILE *fp;
    fp = fopen(filename, "r");

    if (fp != NULL){
        while ((ch = fgetc(fp)) != EOF)
        {
           if (ch != '\n' ){
                chars++;
           }
        }
        fclose(fp);  
    }
    return chars;

}

Baby* loadStore(){

	char filename[] ="BabyTestMC.txt";

    FILE *fp;
    fp = fopen(filename, "r");

    if(!fp){
        printf("File %s does not exist or you dont have access permissions\n", filename);
        return NULL;
    }
    fclose(fp);

    Baby* pBaby = createBaby();

    int rows = countRows(filename);
    int numChars = countChars(filename);
    int columns;

    if(rows%numChars!=0){
        columns = numChars/rows;
    }else{
        printf("Invalid file format. Please have equal column size for each row\n");
        return NULL;
    }

    if(columns != NUMBEROFADDRESSES){
        printf("Invalid file format. Lines must be %d bits \n", NUMBEROFADDRESSES);
    }
    if(rows > MEMSIZE){
        printf("Invalid file format. Maximum of %d lines", MEMSIZE);
    }

    fp = fopen(filename, "r");

    if (fp != NULL){
        
        
        char line[NUMBEROFADDRESSES];
        int i =0;

            while (fgets(line, 60, fp) != NULL){

                for (int j=0; j<NUMBEROFADDRESSES; j++){
                
                    pBaby->store[i][j] = line[j];
                }
            i++;
            if (i==(rows)){
            	break;
            }
            }
        
        fclose(fp);
        
    }else{
        printf("File %s not found!\n", filename );
    }
    
    return pBaby;

}



Baby* fetch(Baby* pBaby){
	//CI tells you the line in store to fetch
	//copy this line in store into the PI
	int line = binToDec(pBaby->controlInstruction, MEMSIZE);
	line --;
	for (int i = 0; i < NUMBEROFADDRESSES; i++) {
		pBaby->presentInstruction[i] = pBaby->store[line][i];
	}	
	return pBaby;

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
