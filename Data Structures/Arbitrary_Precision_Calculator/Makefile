CFLAGS=-Wall -g

OUTPUT=APC # Your output executeable name

OBJ=$(patsubst %.c,%.o,$(wildcard *.c))

$(OUTPUT) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJ) $(OUTPUT)
