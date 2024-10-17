#include "data.h"

#define MAX_NUMBER 16383 /* Maximum positive value for 15-bit signed integer */
#define MIN_NUMBER -16384 /* Minimum negative value for 15-bit signed integer */

/* 
	Function: createData
	Creates a new Data object, initializes it with the given number and line number,
	and converts the number to its 15-bit binary representation.

	Parameters:
		number - The integer value to store in the Data object.
		lineNumber - The line number associated with this Data object.

	Returns:
		A pointer to the newly created Data object.
*/
Data* createData(int number, int lineNumber)
{
	Data* newData = (Data*)malloc(sizeof(Data));
	if (newData == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for new Data\n");
		exit(EXIT_FAILURE);
	}

	newData->number = number;
	newData->lineNumber = lineNumber;
    
	/* Convert number to binary representation */
	newData->numberValue = to_15bit_binary(number);

	newData->next = NULL;

	return newData;
}

/* 
	Function: addData
	Adds a new Data object to the end of the list. 

	Parameters:
		head - A pointer to the pointer of the head of the Data list.
		number - The integer value to store in the new Data object.
		lineNumber - The line number associated with this Data object.
*/
void addData(Data** head, int number, int lineNumber)
{
	Data* current = *head;
	Data* newData;
	newData = createData(number, lineNumber); 
	
	if (*head == NULL) 
	{
		*head = newData;
		return;
	}

	while (current->next != NULL) 
	{
		current = current->next;
	}

	current->next = newData;
}

/* 
	Function: freeDataList
	Frees all memory allocated for the Data list.

	Parameters:
		head - A pointer to the head of the Data list.
*/
void freeDataList(Data* head)
{
	Data* current = head;
	Data* nextNode;
	while (current != NULL)
	{
		nextNode = current->next;
		free(current);
		current = nextNode;
	}
}

/* 
	Function: printDataList
	Prints the entire Data list.

	Parameters:
		head - A pointer to the head of the Data list.
*/
void printDataList(Data* head)
{
	Data* current = head;
	while (current != NULL)
	{
		printf("Number: %d\n", current->number);
		printf("Line Number: %d\n", current->lineNumber);
		printf("15-bit Binary Representation: ");
		printBinary(current->numberValue, 15); /* Print as binary */
		putchar('\n');
		current = current->next;
	}
}

/* 
	Function: createCharData
	Creates a new CharData object, initializes it with the given character and line number,
	and computes the ASCII value of the character.

	Parameters:
		character - The character to store in the CharData object.
		lineNumber - The line number associated with this CharData object.

	Returns:
		A pointer to the newly created CharData object.
*/
CharData* createCharData(char character, int lineNumber)
{
	CharData* newCharData = (CharData*)malloc(sizeof(CharData));
	if (newCharData == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for new CharData\n");
		exit(EXIT_FAILURE);
	}
	newCharData->character = character;
	newCharData->lineNumber = lineNumber;
	newCharData->asciiValue = (unsigned char)character; 
	newCharData->next = NULL;
	return newCharData;
}

/* 
	Function: addCharData
	Adds a new CharData object to the end of the list.

	Parameters:
		head - A pointer to the pointer of the head of the CharData list.
		character - The character to store in the new CharData object.
		lineNumber - The line number associated with this CharData object.
*/
void addCharData(CharData** head, char character, int lineNumber)
{
	CharData* newCharData;
	CharData* current = *head;
	newCharData = createCharData(character, lineNumber); 

	if (*head == NULL) 
	{
		*head = newCharData;
		return;
	}

	while (current->next != NULL) 
	{
		current = current->next;
	}

	current->next = newCharData;
}

/* 
	Function: printCharDataList
	Prints the entire CharData list.

	Parameters:
		head - A pointer to the head of the CharData list.
*/
void printCharDataList(CharData* head)
{
	CharData* current = head;
	while (current != NULL)
	{
		printf("Character: %c\n", current->character);
		printf("Line Number: %d\n", current->lineNumber);
		printf("ASCII Value: %d\n", current->asciiValue);
		current = current->next;
	}
}

/* 
	Function: freeCharDataList
	Frees all memory allocated for the CharData list.

	Parameters:
		head - A pointer to the head of the CharData list.
*/
void freeCharDataList(CharData* head)
{
	CharData* current = head;
	CharData* next;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

/* 
	Function: isValidNumber
	Checks if a given number is within the valid range for a 15-bit signed integer.

	Parameters:
		number - The integer value to check.

	Returns:
		1 if the number is valid, otherwise 0.
*/
int isValidNumber(int number)
{
	if (number > MAX_NUMBER || number < MIN_NUMBER)
	{
		return 1; 
	}
	else
	{
		return 0;
	}
}

/* 
	Function: isValidString
	Checks if the provided line is a valid string according to specific rules.
	A valid string must start and end with double quotes.

	Parameters:
		line - The line to validate.
		lineNumber - The line number for error reporting.
		file_name - The name of the file for error reporting.

	Returns:
		1 if the string is invalid, otherwise 0.
*/
int isValidString(const char* line, int lineNumber, char* file_name)
{
	/* Create a modifiable copy of the input line */
	char* trimmedLine = my_strdup(line);
	int length;
	if (trimmedLine == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for trimmed line\n");
		exit(EXIT_FAILURE);
	}

	/* Trim leading and trailing white spaces */
	trim_whitespace_start(trimmedLine);
	trim_whitespace_end(trimmedLine);

	length = strlen(trimmedLine);

	/* Case: Line is too short to be valid */
	if (length < 2)
	{
		printError(ERROR_LINE_TOO_SHORT, lineNumber, file_name);
		free(trimmedLine);
		return 1;
	}

	/* Check if the first and last characters are double quotes */
	if (trimmedLine[0] != '"' && trimmedLine[length - 1] != '"')
	{
		printError(ERROR_MISSING_DOUBLE_QUOTES, lineNumber, file_name);
		free(trimmedLine);
		return 1;
	}

	/* Case 1a: Missing starting double quote, but ending with double quote */
	if (trimmedLine[0] != '"' && trimmedLine[length - 1] == '"')
	{
		printError(ERROR_MISSING_BEGINNING_QUOTE, lineNumber, file_name);
		free(trimmedLine);
		return 1;
	}

	/* Case 1b: Starting with double quote, but missing ending double quote */
	if (trimmedLine[0] == '"' && trimmedLine[length - 1] != '"')
	{
		printError(ERROR_MISSING_END_QUOTE, lineNumber, file_name);
		free(trimmedLine);
		return 1;
	}

	/* If we reach here, the line starts and ends with double quotes */
	free(trimmedLine);
	return 0;
}

/* 
	Function: processValidLine
	Processes a valid line of text by extracting characters and adding them to the CharData list.

	Parameters:
		line - The line of text to process.
		head - A pointer to the pointer of the head of the CharData list.
		lineNumber - The line number associated with this line of text.

	Returns:
		The updated line number after processing.
*/
int processValidLine(const char* line, CharData** head, int lineNumber) 
{
	int i = 0;

	while (line[i] != '\0') 
	{
		addCharData(head, line[i], lineNumber);
		lineNumber++;
		i++;
	}
	if (line[i] == '\0')
	{
		addCharData(head, line[i], lineNumber);
	}
	lineNumber++;
	return lineNumber;
}

/* 
	Function: isNumber
	Checks if the provided string represents a valid number.

	Parameters:
		str - The string to validate.

	Returns:
		1 if the string is a valid number, otherwise 0.
*/
int isNumber(const char* str)
{
	while (*str)
	{
		if (!isdigit(*str) && *str != ' ' && *str != ',')
		{
		return 0; /*not a number*/
		}
		str++;
	}
	return 1; /*is a number*/
}

/* 
 * Process a line of numbers separated by commas.
 * This function processes a line of text containing numbers separated by commas, 
 * ensuring correct formatting and no invalid characters. It also trims unnecessary
 * white spaces and checks for leading/trailing commas and consecutive commas.
 *
 * Parameters:
 * - line: The input line to be processed.
 * - lineNumber: The line number in the file for error reporting.
 * - file_name: The name of the file being processed for error reporting.
 * - no_errors: A pointer to a Boolean flag indicating if any errors occurred.
 *
 * Returns:
 * - A dynamically allocated string with commas replaced by spaces and extra white spaces removed.
 * - NULL if an error occurs.
 */
char* processNumberLine(const char* line, int lineNumber, char *file_name, Bool* no_errors)
{
	char* trimmedLine;
	char* token;
	char* result;
	char* rest;
	char* end;
	char* currentChar;
	int resultSize;
	int prevCharWasComma = 0;
	int length;
	int commaCount = 0;
	int numberCount = 0;
	int isNegativeAllowed = 1;
	int prevCharWasNumber = 0;

	/* Create a modifiable copy of the input line. */
	trimmedLine = my_strdup(line);
	if (trimmedLine == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for trimmed line\n");
		exit(EXIT_FAILURE);
	}

	/* Trim leading white spaces. */
	while (isspace((unsigned char)*trimmedLine))
	{
		trimmedLine++;
	}
	if (*trimmedLine == '\0')
	{
		free(trimmedLine);
		printError(ERROR_INVALID_FORMAT, lineNumber, file_name);
		*no_errors = FALSE;
		return NULL;
	}

	/* Trim trailing white spaces. */
	end = trimmedLine + strlen(trimmedLine) - 1;
	while (isspace((unsigned char)*end) && end > trimmedLine)
	{
		end--;
	}
	*(end + 1) = '\0';

	/* Check for a leading comma. */
	if (trimmedLine[0] == ',')
	{
		free(trimmedLine);
		printError(ERROR_LEADING_COMMA, lineNumber, file_name);
		*no_errors = FALSE;
		return NULL;
	}

	/* Check for a trailing comma. */
	if (trimmedLine[strlen(trimmedLine) - 1] == ',')
	{
		free(trimmedLine);
		printError(ERROR_TRAILING_COMMA, lineNumber, file_name);
		*no_errors = FALSE;
		return NULL;
	}

	/* Check for invalid characters. */
	currentChar = trimmedLine;
	while (*currentChar)
	{
		if (*currentChar == '-')
		{
			if (!isNegativeAllowed)
			{
				/* Negative sign should only be at the start of the number or after a comma */
				free(trimmedLine);
				printError(ERROR_NOT_A_NUMBER, lineNumber, file_name);
				*no_errors = FALSE;
				return NULL;
			}
			isNegativeAllowed = 0; /* Disable negative sign for subsequent characters */
		}
		else if (!isdigit((unsigned char)*currentChar) && !isspace((unsigned char)*currentChar) && *currentChar != ',')
		{
			/* Invalid character found */
			free(trimmedLine);
			printError(ERROR_NOT_A_NUMBER, lineNumber, file_name);
			*no_errors = FALSE;
			return NULL;
		} 
		else if (*currentChar == ',')
		{
			/* Reset flag to allow negative sign for the next number */
			isNegativeAllowed = 1;
			if (prevCharWasComma)
			{
				free(trimmedLine);
				printError(ERROR_CONSECUTIVE_COMMAS, lineNumber, file_name);
				*no_errors = FALSE;
				return NULL; /* Consecutive commas detected */
			}
		}
		else
		{
			/* Valid character */
			isNegativeAllowed = 1;
		}
		prevCharWasComma = (*currentChar == ',');
		prevCharWasNumber = isdigit((unsigned char)*currentChar);
		currentChar++;
	}

	/* Check if the number of commas is correct */
	if (commaCount < numberCount - 1 && numberCount != 2)
	{
		free(trimmedLine);
		printError(ERROR_MISSING_COMMA, lineNumber, file_name);
		return NULL; /* Missing commas between numbers */
	}

	/* Allocate memory for the formatted result. */
	resultSize = strlen(trimmedLine) + 1; /* +1 for spaces */
	result = (char*)malloc(resultSize);
	if (result == NULL)
	{
		fprintf(stderr, "Unable to allocate memory for result\n");
		free(trimmedLine);
		exit(EXIT_FAILURE);
	}

	/* Create the formatted result with spaces instead of commas. */
	result[0] = '\0'; /* Initialize the result string */
	rest = trimmedLine; /* Reset the rest pointer */
	token = strtok(rest, ",");
	while (token != NULL)
	{
		/* Remove unnecessary white spaces */
		while (isspace((unsigned char)*token))
		{
			token++;
		}
		end = token + strlen(token) - 1;
		while (isspace((unsigned char)*end) && end > token)
		{
			end--;
		}
		*(end + 1) = '\0';

		/* Append token and a space to result */
		strcat(result, token);
		strcat(result, " ");

		/* Get next token */
		token = strtok(NULL, ",");
	}

	/* Remove the trailing space */
	length = strlen(result);
	if (length > 0 && result[length - 1] == ' ')
	{
		result[length - 1] = '\0';
	}

	free(trimmedLine);
	return result;
}

/* 
 * Removes the surrounding double quotes from a given string.
 * This function removes the leading and trailing double quotes from a string,
 * allocating a new string with the quotes removed.
 *
 * Parameters:
 * - str: The input string with surrounding double quotes.
 *
 * Returns:
 * - A new dynamically allocated string with the surrounding quotes removed.
 */
char* removeQuotes(const char* str)
{
	int length = strlen(str);

	char* newStr = (char*)malloc(length - 1);
	if (newStr == NULL)
	{
		fprintf(stderr, "Unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	strncpy(newStr, str + 1, length - 3);
	newStr[length - 3] = '\0';

	return newStr;
}

