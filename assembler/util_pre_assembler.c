#include "util_pre_assembler.h"
#include "general_functions.h"

/* Function to check if a line contains only whitespace */
int is_whitespace(const char *line)
{
    	while (*line)
    	{
        	if (!isspace((unsigned char)*line))
        	{
            		return 0;
        	}
        	line++;
    	}
    	return 1;
}

/* Function to check if a macro name is valid */
int is_valid_macro_name(const char *name)
{
	int i;
	/* Array of string literals (array of pointers to characters)*/
    	const char *commands[] = 
	{
		"mov",
		"cmp",
		"add",
		"sub",
		"lea",
        	"clr",
        	"not",
       		"inc",
       		"dec",
        	"jmp",
        	".data",
		".string",
        	"bne",
        	"red",
        	"prn",
        	"jsr",
        	"rts",
        	"stop"
    	};
	for (i = 0; i < 18; i++)
	{
		if (strcmp(name, commands[i]) == 0)
		{
			return 1;
		}
	}
	return 0;	
}

/* Function to find a macro by name in the macro list */
Macro *find_macro(Macro *macro_list, const char *name)
{
    	while (macro_list)
    	{
        	if (strcmp(macro_list->name, name) == 0)
        	{
            		return macro_list;
        	}
        	macro_list = macro_list->next;
    	}
    	return NULL;
}

/* Function to create a new macro */
Macro *create_macro(const char *name)
{
    	Macro *macro = (Macro *)malloc(sizeof(Macro));
    	macro->name = my_strdup(name);
    	macro->lines = NULL;
    	macro->line_count = 0;
    	macro->line_capacity = 0;
    	macro->next = NULL;
    	return macro;
}

/* Function to add a line to a macro */
void add_line_to_macro(Macro *macro, const char *line)
{
    	if (macro->line_count >= macro->line_capacity)
    	{
        	macro->line_capacity = macro->line_capacity == 0 ? 10 : macro->line_capacity * 2;
        	macro->lines = (char **)realloc(macro->lines, macro->line_capacity * sizeof(char *));
    	}
    	macro->lines[macro->line_count] = my_strdup(line);
    	macro->line_count++;
}

/* Function to free memory used by all macros in the list */
void free_all_macros(Macro *head)
{
    	Macro *current = head;
    	while (current != NULL)
    	{
        	Macro *next = current->next;
        	free_macro(current);
        	current = next;
    	}
}
/*Function to free memory of a macro*/
void free_macro(Macro *macro)
{
	int i;
    	if (macro)
    	{
        	free(macro->name);
        	for (i = 0; i < macro->line_count; i++)
        	{
            		free(macro->lines[i]);
        	}
        	free(macro->lines);
        	free(macro);
    	}
}
