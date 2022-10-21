CC := gcc
FLAGS := -Wall -Werror -Wextra -pedantic -g
TARGET := bst

all: main

main: main.o bst.o
	$(CC) $(FLAGS) -o main

bst.o: bst.c
	$(CC) $(FLAGS) -c bst.c

tokenizer: tokenizer_test.c tokenizer.c tokenizer.h
	$(CC) $(FLAGS) tokenizer_test.c tokenizer.c tokenizer.h -o tokenizer
