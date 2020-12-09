/**
*	Struct for linked list storing all lines a symbol is referenced on
**/
struct Line {
	int lineNum;
	struct Line *next;
} line;

/**
*	Struct for symbol table, stored as a linked list with each symbol containing a linked list of lines it is referenced on 
**/
struct Symbol {
	char* name;
	Line *line;
	struct Symbol *next;
} symbol;
