CFLAGS=-Wall -g

OUTPUT=reader # Your output executeable name

OBJ=$(patsubst %.c,%.o,$(wildcard *.c))

$(OUTPUT) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJ) $(OUTPUT)

all: clean $(OUTPUT)
