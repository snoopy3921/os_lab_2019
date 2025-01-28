#!/bin/bash


# Zadanie 11
# To compile program, it requires fin_min_max, sequential_min_max, utils
gcc find_min_max.c sequential_min_max.c utils.c -o main_sequent

# Run the main file with the seed value(value to init random generator) and number of elements
./main_sequent 10 100

# Fork example
gcc fork_example.c -o fork_example
# Test fork example
./fork_example


# Zadanie 22
# To compile program, it requires fin_min_max, parallel_min_max, utils
gcc find_min_max.c parallel_min_max.c utils.c -o main_parallel
# Run the main file in pipe mode
./main_parallel --seed 50 --array_size 100 --pnum 5
# Run the main file in file mode
./main_parallel --seed 50 --array_size 100 --pnum 5 -f

#Zadanie 5
# Create file exec_example.c and compile
gcc exec_example.c -o exec_example
# Run the program
./exec_example