all:simulator assembler

simulator: simulator.c
	gcc -o simulator simulator.c -Wall -Werror -Wextra -pedantic -lm -g

assembler: assembler.c
	gcc -o assembler assembler.c -Wall -Werror -Wextra -pedantic -lm -g
