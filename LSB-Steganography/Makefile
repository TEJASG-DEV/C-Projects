decode.o: decode.c
	gcc -Wall -g -c decode.c -o decode.o
encode.o: encode.c
	gcc -Wall -g -c encode.c -o encode.o

steg: test_encode.c encode.o decode.o
	gcc -Wall -g test_encode.c encode.o decode.o -o steg

clean:
	rm -f steg encode.o decode.o

all: clean steg
