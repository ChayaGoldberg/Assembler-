
#include "first_pass.h"

Operation all_operations_first_pass[] = 
{
    	{"mov", TWO_OPERANDS, {0, 1, 2, 3, -1}, {0, 1, 2, 3, -1},0},
    	{"cmp", TWO_OPERANDS, {0, 1, 2, 3, -1}, {0, 1, 2, 3, -1},1},
    	{"add", TWO_OPERANDS, {1, 2, 3, -1}, {0, 1, 2, 3, -1},2},
    	{"sub", TWO_OPERANDS, {1, 2, 3, -1}, {0, 1, 2, 3, -1},3},
    	{"lea", TWO_OPERANDS, {1, 2, 3, -1}, {1, -1},4},
    	{"clr", ONE_OPERAND, {1, 2, 3, -1}, {-1},5},
    	{"not", ONE_OPERAND, {1, 2, 3, -1}, {-1},6},
    	{"inc", ONE_OPERAND, {1, 2, 3, -1}, {-1},7},
    	{"dec", ONE_OPERAND, {1, 2, 3, -1}, {-1},8},
    	{"jmp", ONE_OPERAND, {1, 2, -1}, {-1},9},
    	{"bne", ONE_OPERAND, {1, 2, -1}, {-1},10},
    	{"red", ONE_OPERAND, {1, 2, 3, -1}, {-1},11},
    	{"prn", ONE_OPERAND, {0, 1, 2, 3, -1}, {-1},12},
    	{"jsr", ONE_OPERAND, {1, 2, -1}, {-1},13},
    	{"rts", NO_OPERANDS, {-1}, {-1},14},
    	{"stop", NO_OPERANDS, {-1}, {-1},15}
};

/* 
	Opens the file for the first pass of processing.
	Parameters:
		- file_name: The name of the file to be processed.
		- IC: Pointer to the instruction counter.
		- DC: Pointer to the data counter.
		- labels: Pointer to the labels list.
		- externList: Pointer to the extern list.
		- entryList: Pointer to the entry list.
		- head: Pointer to the head of the data list.
		- headChar: Pointer to the head of the character data list.
		- instructionArray: Pointer to the array of instructions.
		- label_list_used: Pointer to the list of used labels.
	Returns:
		- A boolean value indicating success (TRUE) or failure (FALSE).
*/

/* Opens the file for the first pass of processing */   
Bool openfileFirstPast(char *file_name, int* IC, int* DC, LabelNode** labels, ExternList* externList,EntryList* entryList,Data**head, CharData** headChar, InstructionArray* instructionArray, Node** label_list_used)
{
	FILE *output_file;
	int length = strlen(file_name)+1;
	char *output_file_name = malloc(length + 3);
	Bool no_errors;
	

	if (output_file_name == NULL)
	{
        	fprintf(stderr,"Unable to allocate memory for file name");
        	exit(EXIT_FAILURE);
    	}

	my_snprintf(output_file_name, (length+3), "%s%s", file_name, ".am");


	output_file = fopen(output_file_name, "r");
       if (output_file == NULL)
       { 
        	fprintf(stderr, "Error opening output_file: %s\n", output_file_name);
            	
            	free(output_file_name);
		 exit(EXIT_FAILURE);
	}
	
	no_errors=processLine(output_file, file_name, IC, DC, labels, externList, entryList, head, headChar, instructionArray, label_list_used);
	fclose(output_file);
	free(output_file_name);
	return no_errors;
}
           
        
/* 
	Processes each line of the file.
	Parameters:
		- file: Pointer to the file to be processed.
		- file_name: The name of the file being processed.
		- IC: Pointer to the instruction counter.
		- DC: Pointer to the data counter.
		- labels: Pointer to the labels list.
		- externList: Pointer to the extern list.
		- entryList: Pointer to the entry list.
		- head: Pointer to the head of the data list.
		- headChar: Pointer to the head of the character data list.
		- instructionArray: Pointer to the array of instructions.
		- label_list_used: Pointer to the list of used labels.
	Returns:
		- A boolean value indicating success (TRUE) or failure (FALSE).
*/
Bool processLine(FILE* file, char *file_name , int* IC, int* DC, LabelNode** labels, ExternList * externList,EntryList* entryList,Data** head, CharData** headChar, InstructionArray * instructionArray, Node** label_list_used)
{
	/*Setting Variables*/


	int lineNumber = 0;
	char symbolName[LABEL_MAX_LENGTH];
	CommandType nextWord;
	int len;
	char line[MAX_LINE_LENGTH];
	char* token;
	char* firstWord;
	Label* newLabel;
	char* remainingLine;
	char* tempRemainingLine; 
	int number; 
	Bool no_errors=TRUE;
	/*sets the lists*/
	initEntryList(entryList);
    	initExternList(externList);
	init_instruction_array(instructionArray, 2);
	
	/*Reads line by line out of the file*/
	while (fgets(line, sizeof(line), file)) 
	{	lineNumber++;
		
		/*cuts the row into words*/
		token = getFirstWord(line);
        	if (!token)
		{
			
			continue;
        	}
		
		/*cheks if the firts word is a label*/
		if (endsWithColon(token)&& (strlen(token)>1))
		{	
			if (isValidLabel(token,lineNumber,file_name))/*checks that the name is not to long*/
			{	
				free(token);
                		no_errors =FALSE;
                		continue;
			}
			
			
			
			len = strlen(token);
			
			strncpy(symbolName, token, len - 1);
			symbolName[len - 1] = '\0';
	
			if(labelExists(*labels,symbolName))/*checks name dosent exist*/
			{
				
				printError(ERROR_NAME_EXSISTS,lineNumber, file_name);
				no_errors=FALSE;
				free(token);
                		
                		continue;
			}
			
			if(!is_valid_label(symbolName, all_operations_first_pass))
			{
				
				printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL,lineNumber, file_name);
				no_errors=FALSE;
				free(token);              		
                		continue;
			}
			nextWord = nextWordType(line,lineNumber,file_name,no_errors);/*checks what comes after*/
			
			if(nextWord == ERROR)
			{
				free(token);
                		no_errors=FALSE;
                		continue;
			}
			if(nextWord == LABEL)
			{	
				newLabel = createLabel(symbolName, *DC, nextWord);
				addLabel(labels, newLabel);/*adds label to the arry of labels*/
				
				
			}
			else if (nextWord==INSTRUCTION)
			{	
				newLabel = createLabel(symbolName, *IC, nextWord);
                		addLabel(labels, newLabel);/*adds label to the arry of labels*/
				
				
			}
			/*cut line to be remaining line*/
			

			removeFirstWord(line);
			
			token = getFirstWord(line);
			
        		if (!token)
			{
				
				continue;
        		}
			
		}/*finish if label*/



	/*checks if the first word is a Variable*/

	if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0)
	{
		/* Check if the token is ".data" */
		if (strcmp(token, ".data") == 0)
		{
			/* Extract the remaining part of the line after the ".data" token */
			remainingLine = getRemainingLine(line);
			



			if(remainingLine != NULL)
			{
				 /* Process the remaining line to handle numbers */
				remainingLine = processNumberLine(remainingLine, lineNumber, file_name,&no_errors);
				

				if (remainingLine==NULL)
				{
					
					free(remainingLine);
					no_errors=FALSE;
					continue;
				}
			}
			else
			{
				 /* Report error if no numbers follow ".data" */
				printError(ERROR_NO_NUMBER_AFTER_DATA,lineNumber,file_name);
				no_errors=FALSE;
				free(remainingLine);
				
				continue;


			}

			/* Process each number in the remaining line */
			while (remainingLine != NULL)
			{	 
				/* Extract the first word from the remaining line */
				firstWord= getFirstWord(remainingLine);
				
				/* If no more words are found, exit the loop */
				if (firstWord == NULL)
				{
					continue;
				}

				/*Convert firstWord to an integer*/
				number = strtol(firstWord, NULL, 10);
				
				if(isValidNumber(number))
				{	
					/* Report error if the number is not valid */
					printError(ERROR_NOT_VALIED_NUM,lineNumber,file_name);
					no_errors=FALSE;
					remainingLine=NULL;
					continue;
				}
				else
				{
					 /* Add the valid number to the data list and increment DC */
					addData(head,number,*DC); 
					*DC += 1;
					
					
				}
				free(firstWord);
				/* Update remaining line and free old memory */
				tempRemainingLine = getRemainingLine(remainingLine);
				free(remainingLine);
				remainingLine = tempRemainingLine;
				continue;
			}
		
			continue;			
		} 

		/* Check if the token is ".string" */
		else if (strcmp(token, ".string") == 0)
		{	
			remainingLine = getRemainingLine(line);
			
			if(remainingLine == NULL)
			{
				/* Report error if no characters follow ".string" */
				printError(ERROR_NO_CHARS,lineNumber,file_name);
				no_errors=FALSE;
				free(remainingLine);
				continue;
			}

			/* Validate the string in the remaining line */			
			if(isValidString(remainingLine,lineNumber,file_name))
			{	
				 /* If the string is invalid, free memory and continue to the next line */
				free(token);
                		no_errors=FALSE;
                		continue;
			}
			else
			{	
				/* Remove quotes from the string */
				 remainingLine = removeQuotes(remainingLine);
				
				 /* Process the valid line and add characters to the data list */
				 *DC = processValidLine(remainingLine, headChar, *DC);  	
				continue;
   
			 
				
			
			}
		
		}
		
	}
	/*finish if data*/
	
	/*checks if the word is extern or entry*/

	if (strcmp(token, ".extern") == 0)
	{	
		remainingLine = getRemainingLine(line);
			
		if(remainingLine != NULL)
		{	
			/* Trim leading and trailing whitespace from the remaining line */
			trim_whitespace_start(remainingLine);
			trim_whitespace_end(remainingLine);
			
			firstWord= getFirstWord(remainingLine);
			/* Get the remaining part of the line after the first word */
			remainingLine = getRemainingLine(remainingLine);
			
			if(remainingLine != NULL)
			{
				 /* If there is more than one word after ".extern", report an error */
				printError(ERROR_MORE_THEN_1_WORD_AFTER_EXTERN,lineNumber,file_name);
				no_errors=FALSE;
				free(token);
				free(firstWord);
				free(remainingLine);
                		
                		continue;/* Move to the next line */	
			}
			else
			{
				/* Add the extern entry to the extern list */
				addExtern(externList, firstWord, lineNumber);
				
				free(firstWord);
				continue;
			}
		
		}
		else
		{
			 /* If there is no label after ".extern", report an error */
			printError(ERROR_NO_LABEL_AFTER_EXTERN,lineNumber,file_name);
			free(firstWord);
			no_errors=FALSE;
			continue;
		}	
	} 
	else if (strcmp(token, ".entry") == 0)
	{	
		/* Get the remaining part of the line after the ".entry" token */
		remainingLine = getRemainingLine(line);
			
		if(remainingLine != NULL)
		{
			/* Trim leading and trailing whitespace from the remaining line */
			trim_whitespace_start(remainingLine);
			trim_whitespace_end(remainingLine);
			
			/* Get the first word from the remaining line */
			firstWord= getFirstWord(remainingLine);
			/* Get the remaining part of the line after the first word */
			remainingLine = getRemainingLine(remainingLine);
			if(remainingLine != NULL)
			{
				 /* If there is more than one word after ".entry", report an error */
				printError(ERROR_MORE_THEN_1_WORD_AFTER_ENTRY,lineNumber,file_name);
				no_errors=FALSE;
				free(token);
				free(firstWord);
				free(remainingLine);
                		
                		continue;/* Move to the next line */	
			}
			else
			{
				 /* Add the entry to the entry list */
				addEntry(entryList, firstWord, lineNumber);
				free(firstWord);
				free(remainingLine);
				continue;
			}
			
		}
		else
		{
			/* If there is no label after ".entry", report an error */
			printError(ERROR_NO_LABEL_AFTER_ENTRY,lineNumber,file_name);
			no_errors=FALSE;
			free(firstWord);
			free(remainingLine);
			continue;
		}
		
	
		
	}
	
	/* Call the function to check if its an instruction */
        if (!check_operation(line, lineNumber, IC, instructionArray,label_list_used,file_name))  
        {
      
		    no_errors=FALSE;
        }
	
	

        /* Close the file*/
       

	free(token);
		 /* Free token after processing */
		
	}
	
  	return no_errors;
	/* Free labels after processing */
	
	
}



	


	





























	



