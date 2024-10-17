#include "instructions.h"
#include "general_functions.h"
#include "util_instructions.h"


/* 
 * Function to check if a given method is valid based on an array of valid methods.
 * 
 * This function iterates through an array of valid methods and checks if the provided
 * method is included in the array. The array of valid methods must be terminated by 
 * a sentinel value of -1 to indicate the end of the list.
 * 
 * @param method: The method to check for validity.
 * @param valid_methods: An array of valid methods, ending with -1 to mark the end of the list.
 * 
 * @return: TRUE if the method is found in the array of valid methods, FALSE otherwise.
 */
int is_valid_method(int method, int valid_methods[]) 
{
    int i;
   
    for (i = 0; valid_methods[i] != -1; i++) 
    {
        
        if (method == valid_methods[i]) 
        {
            return TRUE;
        }
    }
    return FALSE;
}




/* 
 * Function to get the addressing method from the operand.
 * 
 * Parameters:
 *     operand - The operand string to be analyzed.
 *     extraValue - A pointer to an integer that will store the extra value 
 *                  (immediate value or register number).
 * 
 * Returns:
 *     The addressing method:
 *     - 0 for immediate addressing
 *     - 1 for direct addressing
 *     - 2 for indirect register addressing
 *     - 3 for direct register addressing
 * 
 * Additionally, if the addressing method is immediate (0), 
 * the immediate value will be stored in *extraValue.
 * If the addressing method is register direct (3) or indirect (2),
 * the register number (0-7) will be stored in *extraValue.
 */
int get_addressing_method(char *operand, int *extraValue) 
{
	int i = 1;
	*extraValue = 0;

    	if (operand[0] == '#') 
    	{
        	/* Immediate addressing */
        
        	/* Check for optional '+' or '-' sign */
        	if (operand[1] == '+' || operand[1] == '-') 
        	{
            		i = 2; /* Skip the sign */
        	}
        	/* Ensure that the rest are digits */
        	
        	while (operand[i] != '\0') 
        	{
            		if (!isdigit(operand[i])) 
            		{
                		return -1; /* Invalid immediate operand */
            		}
            		*extraValue = *extraValue * 10 + (operand[i] - '0');
            		i++;
        	}
        	/* Adjust for the sign if necessary */
        	if (operand[1] == '-') 
        	{
            		*extraValue = -*extraValue;
        	}
        	return 0; /* Valid immediate addressing */
    	}
    	else if (operand[0] == 'r' && isdigit(operand[1]) && operand[2] == '\0' && operand[1] >= '0' && operand[1] <= '7') 
    	{
        	/* Direct register addressing */
        	*extraValue = operand[1] - '0'; /* Extract the register number */
        	return 3; /* Direct register addressing */
    	}
    	else if (operand[0] == '*' && operand[1] == 'r' && isdigit(operand[2]) && operand[3] == '\0' && operand[2] >= '0' && operand[2] <= '7') 
    	{
        	/* Indirect register addressing */
        	*extraValue = operand[2] - '0'; /* Extract the register number */
        	return 2; /* Indirect register addressing */
    	}
    	else 
    	{
        	return 1; /* Direct addressing */
    	}
}


/* Function to validate if a label is valid */
Bool is_valid_label(char *label,Operation *all_operations) 
{
  	int i;
	for (i = 0; i < 16; i++) 
  	{
		if (strcmp(label, all_operations[i].name) == 0) 
    		{
      			return FALSE;
		}
    	}

    	/* Ensure the first character is a letter */
    	if (!isalpha(label[0])) 
	{
        	return FALSE;
    	}

    	/* Ensure all characters are either letters or digits */
    	for (i = 1; label[i] != '\0'; i++) 
	{
        	if (!isalnum(label[i])) 
		{
            		return FALSE;
        	}
    	}

	

    	return TRUE;
}

/* 
 * Initializes an instruction array with a specified initial capacity.
 *
 * Parameters:
 *     array - A pointer to the InstructionArray to be initialized.
 *     initial_capacity - The initial number of instructions that the array can hold.
 */
void init_instruction_array(InstructionArray *array, size_t initial_capacity) 
{
        array->instructions = malloc(initial_capacity * sizeof(Instruction));
        array->size = 0;
        array->capacity = initial_capacity;
}

/* 
 * Adds a detailed encoded instruction to the instruction array.
 * If the array's capacity is reached, it is doubled to accommodate more instructions.
 *
 * Parameters:
 *     array - A pointer to the InstructionArray where the instruction will be added.
 *     instr - The EncodedInstruction to be added to the array.
 */
void add_detailed_instruction(InstructionArray *array, EncodedInstruction instr) 
{	
        if (array->size >= array->capacity) {
                array->capacity *= 2;
                array->instructions = realloc(array->instructions, array->capacity * sizeof(Instruction));
        }
        array->instructions[array->size].instruction.detailed = instr;
        array->instructions[array->size].isRaw = 0;  /* Indicates that the instruction is detailed */  
	array->instructions[array->size].isSimple = 0;  /* Indicates that the instruction is detailed */     
	array->size++;
}



/* 
 * Frees the memory allocated for the instruction array and resets its fields.
 * This function should be called when the array is no longer needed to avoid memory leaks.
 *
 * Parameters:
 *     array - A pointer to the InstructionArray to be freed.
 */
void free_instruction_array(InstructionArray *array) 
{
        free(array->instructions);
        array->instructions = NULL;
        array->size = 0;
        array->capacity = 0;
}

/* 
 * Prints the details of an encoded instruction.
 * The printed information includes the opcode, source operand, destination operand, and ARE fields.
 *
 * Parameters:
 *     instruction - The EncodedInstruction to be printed.
 */
void print_instruction(Instruction *inst) 
{
    uint16_t value = 0;
    if (inst->isRaw) 
    {
        /* Print raw instruction's ARE and num fields */
	 printf("RAW Instruction:\n");
        printf("ARE: %u, Num: %u\n", inst->instruction.raw.ARE, inst->instruction.raw.num);
    } 

    else if (inst->isSimple) 
    {
        /* Print simple instruction details */
        printf("Simple Instruction:\n");
        printf("  ARE: 0x%X\n", inst->instruction.simple.ARE);
        printf("  Destination Operand: 0x%X\n", inst->instruction.simple.destOperand);
        printf("  Source Operand: 0x%X\n", inst->instruction.simple.srcOperand);
    } 
    else 
    { 
	  
        /* Print detailed instruction encoding */
	printf("detailed Instruction:\n");        
        value |= (inst->instruction.detailed.ARE & 0x7) << 12;
        value |= (inst->instruction.detailed.srcOperand & 0xF) << 8;
        value |= (inst->instruction.detailed.destOperand & 0xF) << 4;
        value |= (inst->instruction.detailed.opcode & 0xF);
        
        /* Print the encoded instruction in binary format */
        print_binary(value);
    }
}





/* 
 * Prints the binary representation of a 15-bit encoded instruction.
 * The printed format is: opcode, destination operand, source operand, ARE (each in its own 4-bit or 3-bit segment).
 *
 * Parameters:
 *     value - The 15-bit encoded instruction to be printed in binary format.
 */
void print_binary(uint16_t value) 
{
        /* Extract each part from the 15-bit encoded instruction */
        uint16_t opcode = value & 0xF;        /* 4 bits for opcode */
        uint16_t destOperand = (value >> 4) & 0xF;  /* 4 bits for destination operand */
        uint16_t srcOperand = (value >> 8) & 0xF;   /* 4 bits for source operand */
        uint16_t ARE = (value >> 12) & 0x7;    /* 3 bits for ARE */
        int i;

        /* Print the binary representation in the required order */

        /* Print the 4 bits for opcode */
        for (i = 3; i >= 0; i--) 
	{
                putchar((opcode & (1 << i)) ? '1' : '0');
        }
        putchar(' '); /* Space between parts */

	
        /* Print the 4 bits for source operand */
        for (i = 3; i >= 0; i--) 
	{
                putchar((srcOperand & (1 << i)) ? '1' : '0');
        }
        putchar(' '); /* Space between parts */


        /* Print the 4 bits for destination operand */
        for (i = 3; i >= 0; i--) 
	{
                putchar((destOperand & (1 << i)) ? '1' : '0');
        }
        putchar(' '); /* Space between parts */
        
        /* Print the 3 bits for ARE */
        for (i = 2; i >= 0; i--) 
	{
                putchar((ARE & (1 << i)) ? '1' : '0');
        }   
        putchar('\n'); /* Newline at the end */
}



/**
 * Appends a new node to the end of a linked list.
 * 
 * Allocates memory for a new node and initializes its fields with the provided values.
 * The new node is added to the end of the linked list. If the list is empty, the new
 * node becomes the head of the list.
 * 
 * @param head_ref: A pointer to a pointer to the head of the list. This allows for 
 *                  modification of the head pointer if the list is empty.
 * @param line_number: The line number to be stored in the new node.
 * @param label: A string representing the label to be stored in the new node.
 * @param ic: An integer value representing the IC (Instruction Counter) to be stored in the new node.
 */
void append_node(Node **head_ref, int line_number, const char *label, int ic) 
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    Node *last;
    if (new_node == NULL) 
    {
        fprintf(stderr,"Failed to allocate memory for new node");
        return;
    }
    
    new_node->ic = ic;
    new_node->line_number = line_number;
    new_node->line_text = my_strdup(label);
    if (new_node->line_text == NULL) 
    {
        fprintf(stderr,"Failed to duplicate string");
        free(new_node);
        return;
    }
    new_node->next = NULL;
    
    if (*head_ref == NULL) 
    {
        *head_ref = new_node;
        return;
    }
   
    last = *head_ref;
    while (last->next != NULL)
    {
        last = last->next;
    }
    
    last->next = new_node;
}

/**
 * Prints the contents of a linked list of nodes.
 * 
 * Iterates through the linked list and prints the line number, IC, and label for each node.
 * 
 * @param head: A pointer to the head of the linked list.
 */
void print_list(Node *head) 
{
    Node *current = head;
    while (current != NULL) 
    {
        printf("Line %d: ic:%d label:%s\n", current->line_number, current->ic, current->line_text);
        current = current->next;
    }
}

/**
 * Frees all nodes in a linked list and their associated resources.
 * 
 * Iterates through the linked list, frees the memory allocated for each node's string
 * and then frees the node itself. Moves to the next node before freeing the current node.
 * 
 * @param head: A pointer to the head of the linked list.
 */
void free_list(Node* head) 
{
    Node* current = head;
    Node* next;
    while (current != NULL) 
    {
        next = current->next;
        free(current->line_text); /* Free the string */
        free(current); /* Free the node */
        current = next;
    }
}

/**
 * Prints the contents of an array of instructions.
 * 
 * Iterates through the array of instructions and prints details based on the type of
 * each instruction. Instructions can be raw, simple, or detailed.
 * 
 * @param array: A pointer to the `InstructionArray` struct containing the instructions.
 */
void printInstructionArray(const InstructionArray *array) 
{
    size_t i;
    
    /* Iterate over all instructions in the array */
    for (i = 0; i < array->size; i++) 
    {
        /* Pointer to the current instruction */
        const Instruction *inst = &array->instructions[i];

        /* Check if the instruction is of type raw */
        if (inst->isRaw) 
        {
            /* Print the raw value of ARE and num */
            printf("Instruction %lu (Raw): ARE: %u, Num: 0x%03X\n", 
                   (unsigned long)(i + 1), 
                   inst->instruction.raw.ARE, 
                   inst->instruction.raw.num);
        } 
        else if (inst->isSimple) 
        {
            /* Print the details of the simple instruction */
            printf("Instruction %lu (Simple):\n", (unsigned long)(i + 1));
            printf("  ARE: 0x%X\n", inst->instruction.simple.ARE);
            printf("  Destination Operand: 0x%X\n", inst->instruction.simple.destOperand);
            printf("  Source Operand: 0x%X\n", inst->instruction.simple.srcOperand);
        } 
        else 
        {
            /* Print the details of the detailed instruction */
            printf("Instruction %lu (Detailed):\n", (unsigned long)(i + 1));
            printf("  Opcode: 0x%X\n", inst->instruction.detailed.opcode);
            printf("  Source Operand: 0x%X\n", inst->instruction.detailed.srcOperand);
            printf("  Destination Operand: 0x%X\n", inst->instruction.detailed.destOperand);
            printf("  ARE: 0x%X\n", inst->instruction.detailed.ARE);
        }
    }
}

/**
 * Adds a new simple instruction to the instruction array.
 * 
 * Checks if the array needs to be resized. If the current capacity is reached,
 * it doubles the capacity of the array. Then, a new `Instruction` is created and
 * initialized with the provided `SimpleInstruction`. The `Instruction` is added to 
 * the end of the array, and the size of the array is incremented.
 * 
 * @param array: A pointer to the `InstructionArray` struct where the new instruction will be added.
 * @param simpleInstruction: The `SimpleInstruction` to be added to the instruction array.
 */
void addSimpleInstruction(InstructionArray *array, SimpleInstruction simpleInstruction) 
{
    Instruction instruction;
    /* Check if we need to resize the array */
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        array->instructions = (Instruction *)realloc(array->instructions, array->capacity * sizeof(Instruction));
        if (array->instructions == NULL) {
            fprintf(stderr,"Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
    }

    /* Create a new Instruction and set its simple field with the simpleInstruction */
    instruction.instruction.simple = simpleInstruction;
    instruction.isRaw = 0;   /* Set the flag to indicate it's not a raw value */
    instruction.isSimple = 1; /* Set the flag to indicate it's a simple value */

    /* Add the new Instruction to the array */
    array->instructions[array->size++] = instruction;
}

/**
 * Adds a new raw instruction to the instruction array.
 * 
 * Checks if the array needs to be resized. If the current capacity is reached,
 * it doubles the capacity of the array. Then, a new `Instruction` is created and
 * initialized with the provided `RawInstruction`. The `Instruction` is added to 
 * the end of the array, and the size of the array is incremented.
 * 
 * @param array: A pointer to the `InstructionArray` struct where the new instruction will be added.
 * @param rawInstruction: The `RawInstruction` to be added to the instruction array.
 */
void addRawInstruction(InstructionArray *array, RawInstruction rawInstruction) 
{
    Instruction instruction;
    /* Check if we need to resize the array */
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        array->instructions = (Instruction *)realloc(array->instructions, array->capacity * sizeof(Instruction));
        if (array->instructions == NULL) {
            fprintf(stderr,"Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
    }

    /* Create a new Instruction and set its raw field with the rawInstruction */
    instruction.instruction.raw = rawInstruction;
    instruction.isRaw = 1;   /* Set the flag to indicate it's a raw value */
    instruction.isSimple = 0; /* Set the flag to indicate it's not a simple value */

    /* Add the new Instruction to the array */
    array->instructions[array->size++] = instruction;
}












