#include "assembler.h"

/* 
	Processes the given file(s) by calling the appropriate functions.
	If no files are provided, the program will terminate with an error message.
*/


int main(int argc, char *argv[])
{    
    	int i;
	
    	/* 
			Check if there are any files to read from.
			If no files are provided, print an error message and exit.
		*/
    	if(argc < 2)
    	{
        	fprintf(stderr, "Error: there is no file to read from\n");
        	return 1;
    	}

    	/* 
			Iterate through all provided files and process each one.
		*/
    	for(i = 1; i < argc; i++)
    	{	
        	process_file(argv[i]);
    	}
    	
		/* 
			Program completed successfully.
		*/
    	return 0;
}

/* 
	Processes a single file by performing the first and second passes over it.
	Initializes necessary structures and checks for errors during processing.
	If macro_file processing fails, an error message is printed and the function returns early.
*/
void process_file(char *file_name)
{
		Bool no_errors = TRUE;
		int IC = 100;
		int DC = 0;
		LabelNode* labels = NULL;
		ExternList externList;
		InstructionArray instructionArray;
		Node* label_list_used = NULL;
		EntryList* entryList = (EntryList*)malloc(sizeof(EntryList));
		Data* head = NULL;
		CharData* headChar = NULL;

		/* Initialize the lists and instruction array */
		initEntryList(entryList);
		initExternList(&externList);
		init_instruction_array(&instructionArray, 2);

		/* 
			Check if the file contains macros and process it if true.
			Perform the first pass and then the second pass over the file.
		*/
		if (macro_file(file_name))
		{
				no_errors = openfileFirstPast(file_name, &IC, &DC, &labels, &externList, entryList, &head, &headChar, &instructionArray, &label_list_used);
				secondPass(file_name, &IC, &DC, &labels, &externList, &entryList, &head, &headChar, &instructionArray, &label_list_used, &no_errors);
		}
		else
		{
				/* Print an error message if the file processing fails */
				fprintf(stderr, "Failed to process file: %s\n", file_name);
				return;
		}
}

