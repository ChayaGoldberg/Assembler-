#ifndef FIRST_PAST_H
#define FIRST_PAST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "label.h"
#include "data.h"
#include "entry_extern.h"
#include "general_functions.h"
#include "instructions.h"
#include "util_instructions.h"

#define LABEL_MAX_LENGTH 32

/*
	Function: openfileFirstPast
	--------------------------
	Opens the specified file for reading and initializes the necessary structures 
	for the first pass of the assembler.
	
	Parameters:
		file_name - The name of the file to open.
		IC - Pointer to the Instruction Counter.
		DC - Pointer to the Data Counter.
		labels - Pointer to the list of labels.
		externList - Pointer to the list of external labels.
		entryList - Pointer to the list of entry labels.
		head - Pointer to the head of the data list.
		headChar - Pointer to the head of the character data list.
		instructionArray - Pointer to the array of instructions.
		label_list_used - Pointer to the list of used labels.

	Returns:
		Bool - TRUE if the file was successfully opened and processed; otherwise FALSE.
*/
Bool openfileFirstPast(char *file_name, int* IC, int* DC, LabelNode** labels, ExternList* externList, EntryList* entryList, Data** head, CharData** headChar, InstructionArray* instructionArray, Node** label_list_used);

/*
	Function: processLine
	---------------------
	Processes a single line from the input file during the first pass of the assembler.
	
	Parameters:
		file - The file pointer to the input file.
		file_name - The name of the file being processed.
		IC - Pointer to the Instruction Counter.
		DC - Pointer to the Data Counter.
		labels - Pointer to the list of labels.
		externList - Pointer to the list of external labels.
		entryList - Pointer to the list of entry labels.
		head - Pointer to the head of the data list.
		headChar - Pointer to the head of the character data list.
		instructionArray - Pointer to the array of instructions.
		label_list_used - Pointer to the list of used labels.

	Returns:
		Bool - TRUE if the line was successfully processed; otherwise FALSE.
*/
Bool processLine(FILE* file, char *file_name , int* IC, int* DC, LabelNode** labels, ExternList * externList, EntryList* entryList, Data** head, CharData** headChar, InstructionArray * instructionArray, Node** label_list_used);

/*
	Function: isValidOperation
	--------------------------
	Checks if the operation in the provided token is valid according to the assembly language specification.
	
	Parameters:
		token - The token representing the operation to validate.
		lineNumber - The current line number in the file being processed.

	Returns:
		int - The index of the operation if valid; otherwise -1.
*/
int isValidOperation(const char *token, int lineNumber);

#endif /* FIRST_PAST_H */

