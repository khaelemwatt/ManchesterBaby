/**
*	Struct for linked list storing all lines a symbol is referenced on
**/
typedef struct line {
	int lineNum;
	struct line *next;
} Line;


/**
*	Struct for symbol table, stored as a linked list with each symbol containing a linked list of lines it is referenced on 
*/
typedef struct symbol {
	char* name;
	Line *line;
	struct symbol *next;
} Symbol;
