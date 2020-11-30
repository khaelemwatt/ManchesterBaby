#include <stdio.h>
#include <stdlib.h>
#include "header.h"

//Initial global declarations
int accumulator[MEMSIZE];
int controlInstruction[MEMSIZE];
int presentInstruction[MEMSIZE];
int store[MEMSIZE][ADDRESSSIZE];
int addressBus[MEMSIZE];
int controlBus[MEMSIZE]

void incrementCI(){
	//Increment the control instruction by 1
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

void display(){
	//Display the contents of the data store
}

int main(){ //Main loop for the fetch execute cycle

	incrementCI();
	fetch();
	decode();
	execute();
	display();
}
