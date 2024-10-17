#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include <stdio.h>     /* Standard input/output definitions */
#include <ctype.h>    /* Character handling functions */
#include <stdlib.h>   /* Standard library definitions */
#include <string.h>   /* String manipulation functions */
#include "general_functions.h"  /* General utility functions */
#include "util_pre_assembler.h" /* Utility functions specific to the pre-assembler */

/* Define constants for various lengths and file extensions */
#define MAX_LINE_LENGTH 81         /* Maximum length of a line of text */
#define END_OF_FILE 3              /* Length of file extension for .as and .am */
#define END_OF_TEMP_FILE 5         /* Length of file extension for .temp */
#define LENGTH_MACR 4              /* Length of the string "macro" */
#define LENGTH_ENDMACR 7           /* Length of the string "endmacro" */

/**
 * Processes a file with macros, creating temporary and output files. 
 * Handles macro replacements and writes results to the output file.
 *
 * @param file_name: The base name of the file to be processed (without extension).
 *
 * @return: TRUE if the processing is successful, otherwise FALSE.
 */
Bool macro_file(char *file_name);

/**
 * Writes non-macro lines from the input file to the output file and 
 * handles macro definitions, updating the macro list as necessary.
 *
 * @param input_file: File pointer for reading the input file.
 * @param output_file: File pointer for writing to the output file.
 * @param macro_list: Pointer to the list of macros to be updated.
 * @param file_name: The base name of the file being processed.
 *
 * @return: TRUE if successful, otherwise FALSE.
 */
Bool write_non_macro_lines(FILE *input_file, FILE *output_file, Macro **macro_list, char* file_name);

/**
 * Replaces macro calls in the input file with their definitions from the macro list 
 * and writes the result to the output file.
 *
 * @param input_file: File pointer for reading the temporary file with macro calls.
 * @param output_file: File pointer for writing the final output file with macros replaced.
 * @param macro_list: List of macros to be used for replacement.
 *
 * @return: TRUE if successful, otherwise FALSE.
 */
Bool replace_macros_in_file(FILE* input_file, FILE* output_file, Macro* macro_list);

#endif /* PRE_ASSEMBLER_H */

