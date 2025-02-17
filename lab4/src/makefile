CC=gcc
CFLAGS=-I.

.PHONY: all 
all : zombie process_memory psum

zombie: zombie.c
	$(CC) -o $@ $^

process_memory: process_memory.c
	$(CC) -o $@ $^ -w

psum: parallel_sum.c
	$(CC) -o $@ $^ -lpthread
clean :
	rm zombie psum process_memory
