CC 		= gcc
CFAGS 	= -g -Wall -Wextra 

main: main.c
	$(CC)  $(CFLAGS) main.c -o main

clean:
	rm -rf main