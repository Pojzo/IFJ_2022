CC := gcc
FLAGS := -Wall -Werror -Wextra -pedantic -g
TARGET := bst

all: main

main: main.o bst.o
	$(CC) $(FLAGS) -o main

bst.o: bst.c
	$(CC) $(FLAGS) -c bst.c

tokenizer: tokenizer_test.c tokenizer.c tokenizer.h utils.c utils.h symtable.c symtable.h
	$(CC) $(FLAGS) tokenizer_test.c tokenizer.c tokenizer.h utils.c symtable.c -o tokenizer
