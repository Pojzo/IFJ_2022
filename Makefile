CC=gcc
CFLAGS=-Werror -Wall -pedantic
DEPS = expression.h list.h parser.h symtable.h tokenizer.h utils.h
OBJ = main.o expression.o list.o parser.o symtable.o tokenizer.o utils.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ifj22: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *o
	rm ifj22
