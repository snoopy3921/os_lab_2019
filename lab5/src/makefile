CC=gcc
CFLAGS=-I.

.PHONY: all run clean
all : mutex cal_mod_parallel

mutex: mutex.c
	$(CC) -o $@ $^ -pthread

cal_mod_parallel: cal_mod_parallel.c
	$(CC) -o $@ $^ -pthread
	
clean :
	rm mutex cal_mod_parallel

run:
	./mutex