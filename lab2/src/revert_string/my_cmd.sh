#!/bin/bash

#To compile main.c
gcc main.c revert_string.c -o main
#Run the program
./main Main_file!

#To compile main.c statically
gcc -c revert_string.c -o revert_string_static.o #Compile object files
ar rcs librevert_string_static.a revert_string_static.o #Create static library
gcc main.c -L. -lrevert_string_static -o main_static #Link with static library
## Note the when create a library .a, prefix lib... is needed, then when include just use -l... (... is the name of lib)
#Run the program
./main Main_file_static!

#To compile main.c dynamically
gcc -fPIC -c revert_string.c -o revert_string_dynamic.o #Compile object files
gcc -shared -o librevert_string_dynamic.so revert_string_dynamic.o #Create shared library
gcc main.c -L. -lrevert_string_dynamic -o main_dynamic #Link with shared library
## Note the when create a library .a, prefix lib... is needed, then when include just use -l... (... is the name of lib)
#Run the program
./main Main_file_dynamic!