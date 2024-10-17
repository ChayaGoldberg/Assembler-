#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>           /* Standard input/output definitions */
#include <ctype.h>          /* Character handling functions */
#include <stdlib.h>         /* Standard library definitions */
#include <string.h>         /* String manipulation functions */

#include "general_functions.h"   /* General utility functions */
#include "pre_assembler.h"       /* Pre-assembler definitions */
#include "util_instructions.h"   /* Utility functions for instructions */

/* Declaration of the array containing operation definitions */
extern Operation operations[];

/**
 * Checks and processes an operation line from the input.
 *
 * @param line: The line of text containing the operation to be checked.
 * @param line_number: The line number where the operation is found.
 * @param ic: Pointer to the instruction counter, updated based on processing.
 * @param instructionArray: Pointer to the array of encoded instructions.
 * @param head_label: Pointer to the head of the linked list of labels.
 * @param file_name: The name of the file being processed.
 *
 * @return: TRUE if the operation line is valid and processed successfully, FALSE otherwise.
 */
Bool check_operation(char *line, int line_number, int *ic, InstructionArray *instructionArray, Node** head_label, char* file_name);

#endif /* INSTRUCTIONS_H */

