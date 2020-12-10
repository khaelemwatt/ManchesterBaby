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

    pBaby->halt = 0;
    pBaby->opcode = 0;
    pBaby->opcode = 0;

    return pBaby;
}

int binToDec(char bin[], int length){
	//Takes an array of chars (binary string) and returns the decimal value
	int decimal = 0;
	if(length==MEMSIZE){
		for(int i=0;i<MEMSIZE-1;i++){
			if(bin[i]=='1'){
				decimal += (int)pow(2, i);
			}
		}
		if(bin[MEMSIZE-1]=='1'){
			return decimal*-1;
		}else{
			return decimal;
		}
	}
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
	int temp = dec;
	int i=0;
	if(dec<0)
		dec=dec*-1;

	while(dec>0){
		remainder = dec % 2;
		dec = dec / 2;
		bin[i] = remainder + '0';
		i++;
	}
	if(temp<0){
		bin[MEMSIZE-1] = '1';
	}else{
		bin[MEMSIZE-1] = '0';
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

int loadStore(Baby* baby, char filename[]){


    FILE *fp;
    fp = fopen(filename, "r");

    if(!fp){
        printf("File %s does not exist or you dont have access permissions\n", filename);
        return 0;
    }
    fclose(fp);

    int rows = countRows(filename);
    int numChars = countChars(filename);
    int columns;

    if(rows%numChars!=0){
        columns = numChars/rows;
    }else{
        printf("Invalid file format. Please have equal column size for each row\n");
        return 0;
    }

    if(columns != NUMBEROFADDRESSES){
        printf("Invalid file format. Lines must be %d bits \n", NUMBEROFADDRESSES);
        return 0;
    }
    if(rows > MEMSIZE){
        printf("Invalid file format. Maximum of %d lines", MEMSIZE);
        return 0;
    }

    fp = fopen(filename, "r");

    if (fp != NULL){        
        
        char line[NUMBEROFADDRESSES];
        int i =0;

        while (fgets(line, 60, fp) != NULL){

            for (int j=0; j<NUMBEROFADDRESSES; j++){
            
                baby->store[i][j] = line[j];
            }

            i++;
            if (i==(rows)){
            	break;
            }
        }
        
        fclose(fp);
        
    }else{
        printf("File %s not found!\n", filename);
        return 0;
    }

    return 1;
}



void fetch(Baby* baby){
	//CI tells you the line in store to fetch
	//copy this line in store into the PI
	int line = binToDec(baby->controlInstruction, MEMSIZE);
	for (int i = 0; i < NUMBEROFADDRESSES; i++) {
		baby->presentInstruction[i] = baby->store[line][i];
	}	
}

void decode(Baby* baby){
	//Get the memory address of the operand and OP code from the current instruction

    char binaryOpcode[3];
    char binaryOperand[4];
    int counter = 0;

    // Get Operand
    for (int i = 0; i < 4; i++){
        binaryOperand[i] = baby->presentInstruction[i];
    }

    // Get Opcode
    for (int j = 13; j < 16; j++){
        binaryOpcode[counter] = baby->presentInstruction[j];
        counter++;
    }

    // Convert values to decimal
    baby->operand = binToDec(binaryOperand, 4); 
    baby->opcode = binToDec(binaryOpcode, 3);
}

void execute(Baby* baby){
	//Carry out the instruction

    // JMP - Set CI to content of Store location
    if (baby->opcode==0){ 
    	for(int i=0;i<4;i++){
    		baby->controlInstruction[i] = baby->store[baby->operand][i];
    	}
    }
    // JRP - Add content of Store location to CI
    if (baby->opcode==1){
        decToBin((binToDec(baby->controlInstruction, MEMSIZE)+baby->operand),baby->controlInstruction);
    }
    // LDN - Load Accumulator with negative form of Store content
    if (baby->opcode==2){ 
       	decToBin((binToDec(getFromStore(baby, baby->operand), MEMSIZE))*-1, baby->accumulator);
    }
	// STO - Copy Accumulator to Store Location
    if (baby->opcode==3){
    	setToStore(baby, baby->operand, baby->accumulator);
    }
    // SUB - Subtract content of store location from accumulator
    if (baby->opcode==4){ 
        decToBin(binToDec(baby->accumulator, MEMSIZE)-binToDec(getFromStore(baby, baby->operand), MEMSIZE), baby->accumulator);
    }
    // SUB - Subtract content of store location from accumulator
    if (baby->opcode==5){
        decToBin(binToDec(baby->accumulator, MEMSIZE)-binToDec(getFromStore(baby, baby->operand), MEMSIZE), baby->accumulator);
    }
    // CMP - Increment CI if Accumulator value is negative, otherwise do nothing
    if (baby->opcode==6){ 
        if (binToDec(baby->accumulator, MEMSIZE) < 0){
            incrementCI(baby);
        }
    }
    // STP - Set Stop lamp and halt machine
    if (baby->opcode==7){
        baby->halt = 1;
    }   
}

void displayBaby(Baby* baby){
	for(int i=0;i<NUMBEROFADDRESSES;i++){
		for(int j=0;j<MEMSIZE;j++){
			printf("%c", baby->store[i][j]);
		}
		printf("\n");
	}
}

char* getFromStore(Baby* baby, int address){
	//Prevents the need for for loops in cdde when loading contents of store
	char *data = malloc(MEMSIZE);
	for(int i=0;i<MEMSIZE;i++){
		data[i] = baby->store[address][i];
	}
	return data;
}

void setToStore(Baby* baby, int address, char data[]){
	//Prevents the need for for loops in code when loading data into store
	for(int i=0;i<MEMSIZE;i++){
		baby->store[address][i] = data[i];
	}
}

int main(){ //Main loop for the fetch decode execute cycle

    Baby* baby = NULL;
    baby = createBaby();
    
    int valid = 0;
    char filename[50];

    while (valid == 0){
        printf("Enter filename: ");
        scanf("%s", filename);
        valid = loadStore(baby, filename);
    }

    while(baby->halt==0){
    	incrementCI(baby);
		fetch(baby);
		decode(baby);
		execute(baby);
		displayBaby(baby);
    }

    free(baby);
}
