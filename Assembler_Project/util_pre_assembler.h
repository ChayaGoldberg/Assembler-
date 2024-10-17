#ifndef UTIL_PRE_ASSEMBLER_H
#define UTIL_PRE_ASSEMBLER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"

#define MAX_LINE_LENGTH 81

/* 
 * Structure to represent a macro.
 * - name: The name of the macro.
 * - lines: Array of lines that the macro contains.
 * - line_count: The current number of lines in the macro.
 * - line_capacity: The allocated capacity for the lines array.
 * - next: Pointer to the next macro in the list.
 */
typedef struct Macro {
    char *name;
    char **lines;
    int line_count;
    int line_capacity;
    struct Macro *next;
} Macro;

/* 
 * Checks if the given line contains only whitespace characters.
 * 
 * @param line: The line to check.
 * @return: 1 if the line contains only whitespace characters, 0 otherwise.
 */
int is_whitespace(const char *line);

/* 
 * Checks if the given macro name is valid according to specific rules.
 * 
 * @param name: The name of the macro to check.
 * @return: 1 if the macro name is valid, 0 otherwise.
 */
int is_valid_macro_name(const char *name);

/* 
 * Finds a macro by its name in the macro list.
 * 
 * @param macro_list: The head of the macro list.
 * @param name: The name of the macro to find.
 * @return: Pointer to the macro if found, NULL otherwise.
 */
Macro *find_macro(Macro *macro_list, const char *name);

/* 
 * Creates a new macro with the given name.
 * 
 * @param name: The name of the new macro.
 * @return: Pointer to the newly created macro.
 */
Macro *create_macro(const char *name);

/* 
 * Adds a line to the specified macro.
 * 
 * @param macro: Pointer to the macro to which the line will be added.
 * @param line: The line to add to the macro.
 */
void add_line_to_macro(Macro *macro, const char *line);

/* 
 * Frees all memory allocated for macros starting from the head of the macro list.
 * 
 * @param head: The head of the macro list.
 */
void free_all_macros(Macro *head);

/* 
 * Frees memory allocated for a single macro.
 * 
 * @param macro: Pointer to the macro to free.
 */
void free_macro(Macro *macro);

#endif /* UTIL_PRE_ASSEMBLER_H */

