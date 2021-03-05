CC = gcc
CFLAGS = -g -Wall

build: main.o header.o
	        $(CC) main.o header.o $(CFLAGS) -o fsminishell

main.o: main.c
	        $(CC) -c main.c
		        
header.o: header.c
	        $(CC) -c header.c

clean: build
	        rm fsminishell *.o

run: build
	        ./fsminishell
