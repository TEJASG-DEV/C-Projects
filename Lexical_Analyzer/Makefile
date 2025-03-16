CFLAGS=-Wall -g

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
OUTPUT = lexer

$(OUTPUT) : $(OBJ)
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJ) $(OUTPUT)
