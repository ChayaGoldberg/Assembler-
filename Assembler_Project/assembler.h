#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pre_assembler.h"
#include "general_functions.h"
#include "first_pass.h"
#include "label.h"
#include "second_pass.h"

/* 
	Processes the given file(s) by calling the appropriate functions.
	If no files are provided, the program will terminate with an error message.
*/
void process_file(char *file_name);

/* 
	Main function that serves as the entry point for the program.
	Processes command-line arguments and initiates file processing.
*/
int main(int argc, char *argv[]);

#endif /* ASSEMBLER_H */

