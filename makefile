main: simulator.c header.h
	gcc -o main simulator.c header.h -Wall -Werror -Wextra -pedantic -lm
