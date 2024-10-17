#include "instructions.h"
#include "general_functions.h"
#include "util_instructions.h"


/* List of operations and their valid addressing methods */
Operation all_operations[] = 
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

/*Checks and processes an operation line from the input.*/
Bool check_operation(char *line, int line_number, int *ic, InstructionArray *instructionArray, Node** head_label, char* file_name) 
{
  char *token;
  char *operation;
  char *source_operand = NULL, *dest_operand = NULL;
  int source_method = 0, dest_method = 0;
  int i;
  size_t len;
  Bool iscomma = FALSE;
  Operation *op = NULL;
  EncodedInstruction encodedInstr;
  SimpleInstruction simpleInstr;
  RawInstruction rawInstr;
  int dst_value = 0;
  int src_value = 0;
  const int MAX_12_BIT = (1 << 12) - 1; /* 32767 */
  const int MIN_12_BIT = -(1 << 12); /* -32768 */
  char *comma_pos;


  /* Extract operation name */
  token = strtok(line, " \t\n");
  if (token == NULL) return FALSE;
  operation = token;

  /* Find the operation in the operations list */
  for (i = 0; i < sizeof(all_operations) / sizeof(all_operations[0]); i++) 
  {
    if (strcmp(operation, all_operations[i].name) == 0) 
    {
      op = &all_operations[i];
      break;
    }
  }

  if (op == NULL) 
  {
    printError(ERROR_NOT_INSTRUCTION, line_number , file_name);
    return FALSE;
  }

  /* Initialize encoded instruction with ARE set to 100, and other fields set to 0 */
  encodedInstr.ARE = 4; /* Set ARE to 100 */
  encodedInstr.destOperand = 0;
  encodedInstr.srcOperand = 0;
  encodedInstr.opcode = op->code;
  /* Initialize simple instruction with ARE set to 100, and other fields set to 0 */
  simpleInstr.reserved=0;
  simpleInstr.ARE=4;
  simpleInstr.srcOperand=0;
  simpleInstr.destOperand=0;
  /* Initialize raw instruction with ARE set to 100, and other fields set to 0 */
  rawInstr.ARE=4;
  rawInstr.num=0;

  /* Check based on the operation type */
  switch (op->type) 
  {
    case NO_OPERANDS:
      /* If operation type is NO_OPERANDS, ensure no additional text exists */
      token = strtok(NULL, " \t\n");
      if (token != NULL) 
      {
        printError(ERROR_EXTRA_TEXT_AFTER_COMMAND, line_number, file_name);
        return FALSE;
      }
      *ic += 1; /* Increment instruction counter */
      
      /* Add the encoded instruction to the array */   
      add_detailed_instruction(instructionArray, encodedInstr);
      break;

    case ONE_OPERAND:
      /* If operation type is ONE_OPERAND, extract the single operand */
      token = strtok(NULL, " \t\n");
      if (token != NULL) 
      {
        trim_whitespace(token);
        dest_operand = token;

        /* Check for a leading comma before the operand */
        if (dest_operand[0] == ',') 
        {
          printError(ERROR_COMMA_BEFORE_OPERAND, line_number, file_name);
          return FALSE;
        }

        /* Check if the last character is a comma */
        len = strlen(dest_operand);
        if (dest_operand[len - 1] == ',') 
        {
          printError(ERROR_COMMA_AFTER_LAST_OPERAND, line_number, file_name);
          return FALSE;
        }

        /* Determine the addressing method for the destination operand */
        dest_method = get_addressing_method(dest_operand, &dst_value);
        /* Check if number is out of range */
        if (dst_value < MIN_12_BIT || dst_value > MAX_12_BIT) 
        {
          printError(ERROR_OUT_OF_RANGE, line_number, file_name);
          return FALSE;
        }

        /* Validate Label (Direct Addressing Method) */
        if ((dest_method == 1 && !is_valid_label(dest_operand,all_operations))) 
        {
                printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, line_number, file_name);
                return FALSE;
        }
	if( dest_method == -1)
	{
				
                printError(ERROR_INVALID_OPERATION, line_number, file_name);
                return FALSE;
	}
        
        /* Ensure the destination method is valid for the operation */
        if (!is_valid_method(dest_method, op->destMethods)) 
        { 
          printError(ERROR_INVALID_OPERAND_TYPE, line_number, file_name); 
          return FALSE;
        }
	
	/* Check for extra operands */
        token = strtok(NULL, " \t\n");
        if (token != NULL) 
        {
	  	trim_whitespace(token);
		if (token[0] == ',') 
          		printError(ERROR_COMMA_AFTER_LAST_OPERAND, line_number, file_name);
		else
          		printError(ERROR_EXTRA_TEXT_AFTER_OPERANDS, line_number, file_name);
          	return FALSE;
        }

        if (dest_method == 1)
        {
          	append_node(head_label, line_number, dest_operand,(*ic+1));
        }
	
      
        /* Set destination operand in the encoded instruction */
        encodedInstr.destOperand = 1 << dest_method;

        /* Add the encoded instruction to the array */
        add_detailed_instruction(instructionArray, encodedInstr);
      

        /* Handle register direct or indirect addressing */
        if (dest_method == 2 || dest_method == 3) 
        {
    		simpleInstr.destOperand=dst_value;;
		addSimpleInstruction(instructionArray, simpleInstr);
        }
        else
        {
	  rawInstr.num=dst_value;
          addRawInstruction(instructionArray, rawInstr);
        }
      } 
      else 
      {
        printError(ERROR_MISSING_OPERAND, line_number, file_name);
        return FALSE;
      }
      *ic += 2; /* Increment instruction counter */
      break;

    case TWO_OPERANDS:
	 
      	/* If operation type is TWO_OPERANDS, extract the first operand */
      	token = strtok(NULL, " \t\n");
      	if (token != NULL)
      	{
        	trim_whitespace(token);
        	source_operand = token;

        	/* Check for a leading comma before the operand */
        	if (source_operand[0] == ',') 
        	{
          		printError(ERROR_COMMA_BEFORE_OPERAND, line_number, file_name);
          		return FALSE;
        	}

        	/* Check if the last character is a comma and remove it */
        	len = strlen(source_operand);
        	if (source_operand[len - 1] == ',') 
        	{
          		source_operand[len - 1] = '\0';  /* Remove the comma by replacing it with a null terminator */
          		iscomma = TRUE;
        	}

        	/* Check if the first token contains both operands separated by a comma */
        	comma_pos = strchr(source_operand, ',');
        	if (comma_pos != NULL) 
        	{
			 
          		if (iscomma) 
          		{
				printError(ERROR_COMMA_AFTER_LAST_OPERAND, line_number, file_name);
				return FALSE;
			}
            		*comma_pos = '\0'; /* Split the token at the comma */
            		dest_operand = comma_pos + 1;

            	
              		trim_whitespace(dest_operand);
			/* Handle the source operand as usual */
              		source_method = get_addressing_method(source_operand, &src_value);
			if (src_value < MIN_12_BIT || src_value > MAX_12_BIT) 
        		{
          			printError(ERROR_OUT_OF_RANGE, line_number, file_name);
          			return FALSE;
        		}
   			if ((source_method == 1 && !is_valid_label(source_operand,all_operations))) 
              		{
                		printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, line_number, file_name);
                		return FALSE;
              		}
			if( source_method == -1)
			{
				
                		printError(ERROR_INVALID_OPERATION, line_number, file_name);
                		return FALSE;
			}

              		if (!is_valid_method(source_method, op->sourceMethods)) 
              		{
                		printError(ERROR_INVALID_OPERAND_TYPE, line_number, file_name);
                		return FALSE;
             		}

              		if (source_method == 1)
             		{
				
                		append_node(head_label, line_number, source_operand, (*ic+1));
              		}

              		encodedInstr.srcOperand = 1 << source_method;
    
              		/* Handle the destination operand as usual */
              		dest_method = get_addressing_method(dest_operand, &dst_value);
			if (dst_value < MIN_12_BIT || dst_value > MAX_12_BIT) 
        		{
          			printError(ERROR_OUT_OF_RANGE, line_number, file_name);
          			return FALSE;
        		}
              		if ((dest_method == 1 && !is_valid_label(dest_operand,all_operations))) 
              		{
                		printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, line_number, file_name);
                		return FALSE;
              		}
			if( dest_method == -1)
			{
				
                		printError(ERROR_INVALID_OPERATION, line_number, file_name);
                		return FALSE;
			}
              		if (!is_valid_method(dest_method, op->destMethods)) 

              		{
                		printError(ERROR_INVALID_OPERAND_TYPE, line_number, file_name);
                		return FALSE;
              		}

              		if (dest_method == 1)
              		{
                		append_node(head_label, line_number, dest_operand, (*ic+2));
				
              		}
			
              		encodedInstr.destOperand = 1 << dest_method;
			
              		/* Add the encoded instruction to the array */
              		add_detailed_instruction(instructionArray, encodedInstr);
			
              		/* Handle register direct or indirect addressing for source */
              		if (source_method == 2 || source_method == 3) 
              		{
				if (dest_method == 2 || dest_method == 3) 
              			{
					
                			simpleInstr.destOperand=dst_value;
					simpleInstr.srcOperand=src_value;
					addSimpleInstruction(instructionArray, simpleInstr);
					*ic+=2;
					return TRUE;
              			}
				else
				{
					simpleInstr.srcOperand=src_value;
					addSimpleInstruction(instructionArray, simpleInstr);
				}
			}
              		else
              		{
				rawInstr.num=src_value;
          			addRawInstruction(instructionArray, rawInstr);
              		}

              		/* Handle register direct or indirect addressing for destination */
              		if (dest_method == 2 || dest_method == 3) 
              		{
    
    				simpleInstr.destOperand=dst_value;
				addSimpleInstruction(instructionArray, simpleInstr);
    		
              		}
              		else
              		{
                		rawInstr.num=dst_value;
          			addRawInstruction(instructionArray, rawInstr);
              		}	
        	}
        	else
        	{
		
          		/* Comma not found, handle the source operand as usual */
          		source_method = get_addressing_method(source_operand, &src_value);
			if (src_value < MIN_12_BIT || src_value > MAX_12_BIT) 
        		{
          			printError(ERROR_OUT_OF_RANGE, line_number, file_name);
          			return FALSE;
        		}
          		if ((source_method == 1 && !is_valid_label(source_operand,all_operations))) 
              		{
                		printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, line_number, file_name);
                		return FALSE;
              		}
			if( source_method == -1)
			{
				
                		printError(ERROR_INVALID_OPERATION, line_number, file_name);
                		return FALSE;
			}
          		if (!is_valid_method(source_method, op->sourceMethods)) 
          		{
            			printError(ERROR_INVALID_OPERAND_TYPE, line_number, file_name);
            			return FALSE;
          		}

          		/* Handle the destination operand */
          		token = strtok(NULL, " \t\n");
          		if (token == NULL) 
          		{	
            			printError(ERROR_MISSING_OPERAND, line_number, file_name);
            			return FALSE;
			}
            		trim_whitespace(token);
            		dest_operand = token;
			
			/*check for errors*/
            		if (dest_operand[0]==',') 
            		{
				if(iscomma)
				{
              				printError(ERROR_DUPLICATE_COMMA, line_number, file_name);
              				return FALSE;
				}
				if(strlen(dest_operand)==1)
				{
					token = strtok(NULL, " \t\n");	
					if(token==NULL)
					{
						printError(ERROR_MISSING_DEST_OPERAND, line_number, file_name);
						return FALSE;
					}
					trim_whitespace(token);
            				dest_operand = token;
					if (token[0]==',')
					{
              					printError(ERROR_DUPLICATE_COMMA, line_number, file_name);
              					return FALSE;
					}
				}
				else if(dest_operand[1]==',')
				{
					printError(ERROR_DUPLICATE_COMMA, line_number, file_name);
              				return FALSE;
				}
				else
					dest_operand++;

				iscomma=TRUE;
            		}
			if(!iscomma)
			{
				printError(ERROR_MISSING_COMMA_BETWEEN_OPERANDS, line_number, file_name);
              			return FALSE;
			}

            		/* Check if the last character is a comma */
            		len = strlen(dest_operand);
            		if (dest_operand[len - 1] == ',') 
            		{
              			printError(ERROR_COMMA_AFTER_LAST_OPERAND, line_number, file_name);
              			return FALSE;
            		}

            		dest_method = get_addressing_method(dest_operand, &dst_value);
			if (dst_value < MIN_12_BIT || dst_value > MAX_12_BIT) 
        		{
          			printError(ERROR_OUT_OF_RANGE, line_number, file_name);
          			return FALSE;
        		}
            		if ((dest_method == 1 && !is_valid_label(dest_operand,all_operations))) 
              		{
                		printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, line_number, file_name);
                		return FALSE;
              		}
			if( dest_method == -1)
			{
				
                		printError(ERROR_INVALID_OPERATION, line_number, file_name);
                		return FALSE;
			}
            		if (!is_valid_method(dest_method, op->destMethods)) 
            		{
              			printError(ERROR_INVALID_OPERAND_TYPE, line_number, file_name);
              			return FALSE;
            		}

			/* Check for extra operands */
            		token = strtok(NULL, " \t\n");
            		if (token != NULL) 
            		{
              			printError(ERROR_EXTRA_TEXT_AFTER_OPERANDS, line_number, file_name);
              			return FALSE;
            		}

			if (source_method == 1)
          		{
            			append_node(head_label, line_number, source_operand, *ic+1);
          		}

            		if (dest_method == 1)
            		{
              			append_node(head_label, line_number, dest_operand, *ic+2);
            		}

			encodedInstr.srcOperand = 1 << source_method;
            		encodedInstr.destOperand = 1 << dest_method;
			/*add encoded instruction*/
            		add_detailed_instruction(instructionArray, encodedInstr);

             		/* Handle register direct or indirect addressing for source */
              		if (source_method == 2 || source_method == 3) 
              		{
				if (dest_method == 2 || dest_method == 3) 
              			{
                			simpleInstr.destOperand=dst_value;
					simpleInstr.srcOperand=src_value;
					addSimpleInstruction(instructionArray, simpleInstr);
					*ic += 2; /* Increment instruction counter */
      					return TRUE;
              			}
				else
				{
					simpleInstr.srcOperand=src_value;
					addSimpleInstruction(instructionArray, simpleInstr);
				}
			}
              		else
              		{
                		rawInstr.num=src_value;
          			addRawInstruction(instructionArray, rawInstr);
              
              		}

              		/* Handle register direct or indirect addressing for destination */
              		if (dest_method == 2 || dest_method == 3) 
              		{
    
    				simpleInstr.destOperand=dst_value;
				addSimpleInstruction(instructionArray, simpleInstr);
    		
              		}
              		else
              		{
                		rawInstr.num=dst_value;
          			addRawInstruction(instructionArray, rawInstr);
              		}
     
			}
		} 
      		else 
      		{
        		printError(ERROR_MISSING_OPERAND, line_number, file_name);
        		return FALSE;
      		}
      		*ic += 3; /* Increment instruction counter */
      		break;
    	}
  	return TRUE;
}

                               
