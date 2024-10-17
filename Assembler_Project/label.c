#include "label.h"



Label** labels = NULL;
int size = 0;
int capacity = 10;


/* Function to create a new label */
Label* createLabel(const char* name, int lineNumber, CommandType followingContent)
{
    	Label* newLabel = (Label*)malloc(sizeof(Label));
    	if (newLabel == NULL) 
	{
        	fprintf(stderr,"Unable to allocate memory for new label");
        	exit(EXIT_FAILURE);
    	}
    	strncpy(newLabel->name, name, LABEL_MAX_LENGTH - 1);
    	newLabel->name[LABEL_MAX_LENGTH - 1] = '\0';
    	newLabel->lineNumber = lineNumber;
    	newLabel->followingContent = followingContent;
	

    	return newLabel;
}

/* Function to add a label to the linked list */
void addLabel(LabelNode** head, Label* newLabel) 
{
    	LabelNode* newNode = (LabelNode*)malloc(sizeof(LabelNode));
    	if (newNode == NULL) 
	{
        	fprintf(stderr,"Unable to allocate memory for new label node");
        	exit(EXIT_FAILURE);
    	}
    	newNode->label = newLabel;
    	newNode->next = *head;
    	*head = newNode;
}

/* Function to print the labels in the linked list */
void printLabel(const Label* label) 
{
    	if (label == NULL) 
	{
        	printf("Label is NULL\n");
        	return;
    	}

    	printf("Label Name: %s\n", label->name);
    	printf("Line Number: %d\n", label->lineNumber);

    	printf("Following Content: ");
    	switch (label->followingContent) 
	{
        	case LABEL:
            		printf("Label\n");
            		break;
        	case INSTRUCTION:
            		printf("Instruction\n");
            		break;
        	case ERROR:
            		printf("Error\n");
            		break;
        	default:
            		printf("Unknown\n");
            		break;
    	}
	

}


/* Function to print label list */
void printLabelList(const LabelNode* head) {
    const LabelNode* current = head;
    while (current != NULL) {
        printLabel(current->label);
        printf("\n"); 
        current = current->next;
    }
}

/* Function to free the memory used by the linked list */
void freeLabels(LabelNode* head) 
{
    	LabelNode* current = head;
    	while (current != NULL) 	
	{
        	LabelNode* next = current->next;
        	free(current->label);
        	free(current);
        	current = next;
    	}
}


/*checks size of label*/
int isValidLabel(const char* token,int lineNumber,char *file_name)
{
	if(strlen(token)>LABEL_MAX_LENGTH)
	{	
		printError(ERROR_LABEL_TO_LONG,lineNumber,file_name);
		return 1;
	}
	
	return 0;
}

/* Checks if a label exists in the array */
int labelExists(LabelNode* labels, const char* name)
{
	LabelNode* current = labels;
    	while (current) 
	{
        	if (strcmp(current->label->name, name) == 0) 
		{
           		 return 1;
        	}
        	current = current->next;
    	}
    	return 0;
}

/*checks what is the next word*/
CommandType nextWordType(const char* line, int lineNumber,char *file_name, Bool no_errors)
{	
	char *remainingLine;
	char * token;
		 
    	remainingLine = getRemainingLine(line);
	/*checks if there is a word after*/
	if (remainingLine == NULL)
	{
		printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, lineNumber,file_name);
		no_errors =FALSE;
      		return ERROR;
	}

	token = getFirstWord(remainingLine);
	
	free(remainingLine); 

	/*checks the word */
	if (token == NULL)
	{
        	printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL, lineNumber,file_name);
		no_errors =FALSE;
		return ERROR;
	}
	
	if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0)
	{	
		free(token);
		return LABEL;
	}
	
	else if(correctCommand(token)==0)
	{
		free(token);
		return INSTRUCTION;
	}
	else
	{
		free(token);
		printError(ERROR_NOT_VALIED_FORMAT_FOR_LABEL,lineNumber,file_name);
		no_errors =FALSE;
		return ERROR;
	}
}

/*checks if the word ends with ":"*/
int endsWithColon(const char* token)
{
	int len = strlen(token);
	return (len > 0 && token[len - 1] == ':');
}

/*checks if its a type of command*/
int correctCommand(const char *name)
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
        "bne",
        "red",
        "prn",
        "jsr",
        "rts",
        "stop"
        };

	for (i = 0; i < 16; i++)
	{    
        	if(strcmp(name, commands[i]) == 0)
        	return 0;/*Valid command*/
    	}

    return 1; /*Invalid command */   
}







