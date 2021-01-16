
CC = gcc
FLAGS = -Wall
.PHONEY: all clean

all: frequency

frequency: trie.c
	$(CC) $(FLAGS) trie.c -o frequency

clean:
	rm frequency