#include "pre_assembler.h"
#include "general_functions.h"



/* 
 * Processes a file with macros by creating temporary and output files. 
 * It handles macro replacements and writes the results to the output file.
 * 
 */
Bool macro_file(char *file_name)
{
    size_t length = strlen(file_name) + 1;  /* Calculate the length for memory allocation */
    char *input_file_name = malloc(length + END_OF_FILE);  /* Allocate memory for input file name */
    char *output_file_name = malloc(length + END_OF_FILE);  /* Allocate memory for output file name */
    char *temp_file_name = malloc(length + END_OF_TEMP_FILE);  /* Allocate memory for temp file name */
    FILE *input_file;  /* File pointer for reading input file */
    FILE *output_file;  /* File pointer for writing output file */
    FILE *temp_file;  /* File pointer for writing temp file */
    Macro *macro_list = NULL;  /* Pointer to the list of macros */
    Bool success;  /* Variable to indicate success of operations */

    /* Create input, output, and temp file names */
    my_snprintf(input_file_name, (length + 3), "%s%s", file_name, ".as");
    my_snprintf(output_file_name, (length + 3), "%s%s", file_name, ".am");
    my_snprintf(temp_file_name, (length + 5), "%s%s", file_name, ".temp");

    /* Open the input file for reading */
    input_file = fopen(input_file_name, "r");
    if (input_file == NULL)
    { 
        fprintf(stderr, "Error opening input file: %s\n", input_file_name);
        free(input_file_name);
        free(output_file_name);
        free(temp_file_name);
        return FALSE;
    }

    /* Open the output file for writing */
    output_file = fopen(output_file_name, "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_file_name);
        fclose(input_file);
        free(input_file_name);
        free(output_file_name);
        free(temp_file_name);
        return FALSE;
    }

    /* Open the temp file for writing */
    temp_file = fopen(temp_file_name, "w");
    if (temp_file == NULL)
    {
        fprintf(stderr, "Error opening temp file: %s\n", temp_file_name);
        fclose(input_file);
        fclose(output_file);
        free(input_file_name);
        free(output_file_name);
        free(temp_file_name);
        return FALSE;
    }

    /* Initialize macro list */
    macro_list = NULL;

    /* Write non-macro lines to temp file and handle macros */
    success = write_non_macro_lines(input_file, temp_file, &macro_list, file_name);

    /* If there were errors, remove the new files and exit */
    if (!success)
    {
        fclose(input_file);
        fclose(temp_file);
        fclose(output_file);
        remove(temp_file_name);
        remove(output_file_name);
        free(input_file_name);
        free(output_file_name);
        free(temp_file_name);
        if (macro_list)
        {
            free_all_macros(macro_list);
        }
        return FALSE;
    }

    /* Reopen the temp file for reading */
    fclose(temp_file);
    temp_file = fopen(temp_file_name, "r");
    if (temp_file == NULL)
    {
        fprintf(stderr, "Error reopening temp file: %s\n", temp_file_name);
        fclose(input_file);
        fclose(output_file);
        remove(temp_file_name);
        remove(output_file_name);
        free(input_file_name);
        free(output_file_name);
        free(temp_file_name);
        if (macro_list)
        {
            free_all_macros(macro_list);
        }
        return FALSE;
    }

    /* Replace macros in temp file and write to output file */
    success = replace_macros_in_file(temp_file, output_file, macro_list);

    if (macro_list)
    {
        free_all_macros(macro_list);
    }

    /* Close all files and remove temp file */
    fclose(input_file);
    fclose(output_file);
    fclose(temp_file);
    remove(temp_file_name);

    /* Clean up and return success status */
    if (!success)
    {
        remove(temp_file_name);
        remove(output_file_name);
        return FALSE;
    }

    free(input_file_name);
    free(output_file_name);
    free(temp_file_name);

    return TRUE;
}




/* Function to write non-macro lines to the output file and handle macros */
Bool write_non_macro_lines(FILE *input_file, FILE *output_file, Macro **macro_list, char* file_name)
{
	
    	char buffer[MAX_LINE_LENGTH + 2]; /* Buffer to hold lines including the newline character and null terminator */
    	int in_macro = 0;
    	int line_number = 0;
    	Macro *current_macro = NULL;
	Bool success = TRUE;
	char *macro_name_end;
	char *extra_text;
	int c;

    	/* Read lines from the input file */
    	while (fgets(buffer, sizeof(buffer), input_file) != NULL)
    	{
		
		line_number++;

		/* Check if the line length exceeds 81 characters (including \n) */
        	if (strlen(buffer) > MAX_LINE_LENGTH)
        	{
           		printError(ERROR_LINE_TOO_LONG, line_number, file_name);
           		success = FALSE;
            		while ((c = fgetc(input_file)) != EOF && c != '\n');
            			continue;
        	}
		
        	/* Skip comment lines */
        	if (strncmp(buffer, ";", 1) == 0)
        	{
            		continue;
        	}

		trim_whitespace_start(buffer);

		/* Check if the string is empty after trimming */
    		if (buffer[0]=='\0') 
        	{
            		continue; /* Skip empty lines */
        	}
		
        	/* Check for macro definition start */
        	if (strncmp(buffer, "macr",LENGTH_MACR) == 0)
        	{
			
            		char macro_name[MAX_LINE_LENGTH];
            		/* Validate macro name */
            		if (sscanf(buffer, "macr %s", macro_name) != 1 || is_valid_macro_name(macro_name))
            		{
                		printError(ERROR_INVALID_MACRO_NAME, line_number,file_name);
				success = FALSE;
				continue;
            		}

			
            		/* Check for extra text after macro name */
            		macro_name_end = buffer + LENGTH_MACR;
            		while (isspace((unsigned char)*macro_name_end)) macro_name_end++;
            		macro_name_end += strlen(macro_name);
            		while (isspace((unsigned char)*macro_name_end)) macro_name_end++;

            		if (*macro_name_end != '\0' && *macro_name_end != '\n')
            		{
                		printError(ERROR_EXTRA_TEXT_AFTER_MACRO, line_number,file_name);
				success = FALSE;
                		continue;  /*Continue processing the next lines */
            		}

            		/* Check for duplicate macro name */
            		if (find_macro(*macro_list, macro_name))
            		{
                		printError(ERROR_MACRO_ALREADY_EXISTS, line_number,file_name);
				success = FALSE;
                		continue;  /*Continue processing the next lines*/
            		}
			
			/* Check for extra text after macro name */
            		extra_text = strstr(buffer, macro_name) + strlen(macro_name);
            		if (*extra_text != '\0' && !is_whitespace(extra_text))
            		{
                		printError(ERROR_EXTRA_TEXT_AFTER_MACRO, line_number,file_name);
				success = FALSE;
            		}

            		/* Create and add new macro to the list */
            		current_macro = create_macro(macro_name);
            		current_macro->next = *macro_list;
            		*macro_list = current_macro;
            		in_macro = 1;
        
        	}
        	else if (in_macro)
        	{
            		/* Check for macro definition end */
            		if (strncmp(buffer, "endmacr", LENGTH_ENDMACR) == 0)
            		{
                		/* Check for extra text after endmacr */
                		extra_text = buffer + LENGTH_ENDMACR;
                		if (*extra_text != '\0' && !is_whitespace(extra_text))
                		{
                    			printError(ERROR_EXTRA_TEXT_AFTER_ENDMACR, line_number,file_name);
					success = FALSE;
                		}
                		in_macro = 0;
                		current_macro = NULL;
        		}
			else
            		{
                		/* Add line to the current macro */
                		add_line_to_macro(current_macro, buffer); 
            		}
		}
		else if (strncmp(buffer, "endmacr", LENGTH_ENDMACR) == 0)
            	{
                	/* Check for extra text after endmacr */
                	extra_text = buffer + LENGTH_ENDMACR;
               		if (*extra_text != '\0' && !is_whitespace(extra_text))
               		{
                 		printError(ERROR_EXTRA_TEXT_AFTER_ENDMACR, line_number,file_name);
				success = FALSE;
                	}
		}
        	else
        	{
            		/* Write non-macro lines to the output file */
            		fputs(buffer, output_file);
        	}
    	}
	return success;
}





/* 
 * Replaces macros in the input file with their corresponding definitions and writes the result to the output file.
 * 
 */
Bool replace_macros_in_file(FILE* input_file, FILE* output_file, Macro* macro_list) 
{
    char buffer[MAX_LINE_LENGTH];  /* Buffer to hold each line read from the input file */
    int i;  /* Loop counter */
    Macro* macro;  /* Pointer to a macro object */
    char *token;  /* Pointer to the current line of text */

    /* Read each line from the input file */
    while (fgets(buffer, sizeof(buffer), input_file) != NULL) 
    {
        trim_whitespace(buffer);  /* Remove leading and trailing whitespace from the line */
        token = buffer;  /* Set token to the current line */

        if (token != NULL) 
        {
            /* Find the macro corresponding to the current line */
            macro = find_macro(macro_list, token);
            
            if (macro != NULL) 
            {
                /* If a macro is found, write its lines to the output file */
                for (i = 0; i < macro->line_count; i++) 
                {
                    fputs(macro->lines[i], output_file);
                }
            } 
            else 
            {
                /* If no macro is found, write the original line to the output file */
                fputs(buffer, output_file);
                fputc('\n', output_file);  /* Add a newline character after each line */
            }
        }
    }
    return TRUE;  /* Return TRUE indicating the operation was successful */
}




