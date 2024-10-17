# Targets to build object files and final executable
assembler: first_pass.o general_functions.o assembler.o label.o data.o entry_extern.o util_pre_assembler.o instructions.o util_instructions.o pre_assembler.o second_pass.o
	gcc -ansi -pedantic -Wall first_pass.o general_functions.o assembler.o label.o data.o entry_extern.o util_pre_assembler.o instructions.o util_instructions.o pre_assembler.o second_pass.o -o assembler

# Object file rules
first_pass.o: first_pass.c first_pass.h
	gcc -ansi -pedantic -Wall -c first_pass.c -o first_pass.o 

general_functions.o: general_functions.c general_functions.h
	gcc -ansi -pedantic -Wall -c general_functions.c -o general_functions.o

assembler.o: assembler.c assembler.h
	gcc -ansi -pedantic -Wall -c assembler.c -o assembler.o

label.o: label.c label.h
	gcc -ansi -pedantic -Wall -c label.c -o label.o
 
data.o: data.c data.h
	gcc -ansi -pedantic -Wall -c data.c -o data.o

entry_extern.o: entry_extern.c entry_extern.h
	gcc -ansi -pedantic -Wall -c entry_extern.c -o entry_extern.o

instructions.o: instructions.c instructions.h
	gcc -ansi -pedantic -Wall -c instructions.c -o instructions.o

util_instructions.o: util_instructions.c util_instructions.h
	gcc -ansi -pedantic -Wall -c util_instructions.c -o util_instructions.o

pre_assembler.o: pre_assembler.c pre_assembler.h
	gcc -ansi -pedantic -Wall -c pre_assembler.c -o pre_assembler.o 

util_pre_assembler.o: util_pre_assembler.c util_pre_assembler.h
	gcc -ansi -pedantic -Wall -c util_pre_assembler.c -o util_pre_assembler.o

second_pass.o: second_pass.c second_pass.h
	gcc -ansi -pedantic -Wall -c second_pass.c -o second_pass.o

