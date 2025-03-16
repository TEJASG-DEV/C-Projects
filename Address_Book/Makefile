file.o: file.c
	gcc -Wall -g -c file.c -o file.o

contact.o: contact.c
	gcc -Wall -g -c contact.c -o contact.o

contact: main.c contact.o file.o
	gcc -Wall -g main.c contact.o file.o -o contact

clean:
	rm -f contact contact.o file.o

all: clean contact
