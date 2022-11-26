CC := gcc
FLAGS := -Wall -Werror -Wextra -pedantic -g
TARGET := bst

all: main

main: main.o bst.o
	$(CC) $(FLAGS) -o main

bst.o: bst.c
	$(CC) $(FLAGS) -c bst.c

tokenizer: main.c tokenizer.c tokenizer.h utils.c utils.h symtable.c symtable.h parser.c parser.h
	$(CC) $(FLAGS) main.c tokenizer.c tokenizer.h utils.c symtable.c parser.c -o tokenizer

tokrun: tokenizer
	./tokenizer