# basic simple Makefile starter
#
CFLAGS=-Wall -g

read: read.c
	gcc -c read.c -o read.o

logfind: logfind.c read.o
	gcc logfind.c read.o -o logfind

clean:
	rm -f *.o logfind

all: clean logfind
