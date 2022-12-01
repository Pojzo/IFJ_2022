CC := gcc
FLAGS := -Wall -Werror -Wextra -pedantic -g
TARGET := bst

all: main

main: main.o bst.o
	$(CC) $(FLAGS) -o main

bst.o: bst.c
	$(CC) $(FLAGS) -c bst.c

tokenizer: main.c tokenizer.c tokenizer.h utils.c utils.h parser.c parser.h expression.c expression.h list.c list.h
	$(CC) $(FLAGS) main.c tokenizer.c tokenizer.h utils.c parser.c expression.c list.c -o tokenizer
