#include "general_functions.h"

/* Array of error messages corresponding to various error types */
const char* errorMessages[] = {
    "not a valid instruction name",
    "Duplicate symbol definition",
    "Invalid operand",
    "Missing an operand",
    "Invalid operand type",
    "Too many operands",
    "Invalid register",
    "Invalid macro name",
    "Extra text after end macro",
    "Extra text after macro",
    "Macro already exists",
    "Line exceeds the maximum allowed length of 81 characters",
    "Source operand is not allowed for this operation",
    "Destination operand is not allowed for this operation",
    "Invalid addressing mode",
    "Comma missing between operands", 
    "Extra text after operands",
    "Extra text after command",       
    "Missing destination operand",     
    "Missing source operand",
    "Illegal comma before the first operand",
    "There is a duplicate comma",
    "There is an extra comma after the last operand",
    "The immediate number is out of range",
    /* Errors related to label handling */
    "Not a valid label name",
    "Missing instruction",
    "Name of label too long",
    "Number not valid",
    "Unable to allocate memory for new label",
    "Name of label not valid",
    "Name of label exists already",
    /* Errors related to data handling */
    "Missing double quotes at the beginning of the sentence",
    "Missing double quotes at the end of the sentence",
    "Missing double quotes",
    "String is too short",
    "No characters after .string",
    "Invalid format",
    "When entering data of the data type, only numbers are allowed",
    "Consecutive commas between 2 numbers",
    "Missing comma between numbers.",
    "There is a comma at the end of the data",
    "There is a comma at the beginning of the data.",
    "No numbers after data",
    /* Errors related to .entry */
    "No label after .entry",
    "More than one word after .entry",
    /* Errors related to .extern */
    "More than one word after .extern",
    "No label after .extern",
    /* Errors related to the second pass */
    "A label that is external has been defined in the file",
    "A label that is entry was not defined in the file",
    "Label used but not defined",
    "Not enough memory space"
};

/* Function to print an error message with its corresponding line number and file name */
void printError(ErrorType errorType, int lineNumber, char* fileName)
{
    fprintf(stderr, "Error: %s at line %d in file %s\n", errorMessages[errorType], lineNumber, fileName);
}

/* Function to extract and return the first word from a given line of text */
char* getFirstWord(const char* line)
{
    const char* start = line;
    const char* end;
    int length;
    char* firstWord;

    /* Skip leading whitespace */
    while (*start && isspace(*start))
    {
        start++;
    }

    /* If there is no text left after whitespace, return NULL */
    if (*start == '\0')
    {
        return NULL; 
    }

    end = start;

    /* Find the end of the first word */   
    while (*end && !isspace(*end))
    {
        end++;
    }

    /* Calculate the length of the first word */
    length = end - start;
    /* Allocate memory for the first word */
    firstWord = (char*)malloc((length + 1) * sizeof(char));
    if (firstWord == NULL)
    {
        fprintf(stderr,"Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    /* Copy the first word into the newly allocated memory */
    strncpy(firstWord, start, length);
    firstWord[length] = '\0'; /* Add end of string \0 */

    return firstWord;
}

/* Function to extract and return the remaining line after the first word */
char* getRemainingLine(const char* line)
{
    const char* start = line;
    const char* end;
    int length;
    char* remainingLine;

    /* Skip leading whitespace */
    while (*start && isspace(*start))
    {
        start++;
    }

    /* If there is no remaining text, return NULL */
    if (*start == '\0')
    {
        return NULL; 
    }

    end = start;

    /* Find the end of the first word */
    while (*end && !isspace(*end))
    {
        end++;
    }

    /* Skip whitespace after the first word */
    while (*end && isspace(*end))
    {
        end++;
    }

    /* If there's no text remaining after whitespace, return NULL */
    if (*end == '\0')
    {
        return NULL;
    }

    /* Calculate the length of the remaining text */
    length = strlen(end);
    /* Allocate memory for the remaining line */
    remainingLine = (char*)malloc((length + 1) * sizeof(char));
    if (remainingLine == NULL)
    {
        fprintf(stderr,"Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    /* Copy the remaining text into the newly allocated memory */
    strcpy(remainingLine, end);

    return remainingLine;
}

/* Custom implementation to format a string with 2 parameters */
void my_snprintf(char *buffer, size_t size, const char *format, const char *param1, const char *param2) 
{
    /* Clear the buffer */
    buffer[0] = '\0';

    /* Check if buffer is large enough */
    if (size < 1) return;

    /* Construct the formatted string manually */
    if (strstr(format, "%s") != NULL) 
    {
        /* Append first parameter */
        strncat(buffer, param1, size - strlen(buffer) - 1);

        /* Check if buffer is large enough after appending first parameter */
        if (size - strlen(buffer) > 1) 
        {
            /* Append second parameter */
            strncat(buffer, param2, size - strlen(buffer) - 1);
        }
    }
}

/* Function to remove the first word from a line and return the remaining line */
char* removeFirstWord(char* line) 
{
    char *space = strchr(line, ' ');  
    if (space == NULL) {
        return NULL;
    }

    memmove(line, space + 1, strlen(space + 1) + 1);

    return line;
}

/* Function to trim leading whitespace from a string */
void trim_whitespace_start(char* str) 
{
    char *start = str;

    /* Find the first non-whitespace character */
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') 
    {
        start++;
    }

    /* Copy the trimmed string back to the start if necessary */
    if (start != str) 
    {
        strcpy(str, start);
    }
}

/* Function to trim trailing whitespace from a string */
void trim_whitespace_end(char* str) 
{
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) 
    {
        end--;
    }
    *(end + 1) = '\0'; /* Null-terminate the trimmed string */
}

/* Function to duplicate a string, returning a newly allocated copy */
char* my_strdup(const char* str) 
{
    char* copy;
    
    /* Allocate memory for the copy */
    copy = (char*)malloc(strlen(str) + 1); /* +1 for the null terminator */
    
    /* Check if memory allocation was successful */
    if (copy == NULL) {
        fprintf(stderr,"Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    /* Copy the string */
    strcpy(copy, str);
    
    return copy;
}

/* Function to convert an integer to a 15-bit two's complement binary representation. */
uint16_t to_15bit_binary(int number) 
{
    /* Variable to store the result */
    uint16_t result = 0;

    /* Handle negative numbers */
    if (number < 0) 
    {
        /* Convert number to positive and invert bits */
        result = (uint16_t)(-number); 
        result = ~result + 1;
    } 
    else 
    {
        /* For positive numbers, simply store them in result */
        result = (unsigned short int)number;
    }

    /* Mask to ensure only the lowest 15 bits are kept */
    result &= 0x7FFF;

    return result;
}

/* Function to print a binary representation of an unsigned integer with a specified number of bits */
void printBinary(unsigned int value, int bits) 
{
    int i;
    for (i = bits - 1; i >= 0; i--) 
    {
        putchar((value & (1 << i)) ? '1' : '0');
    }
}

/* Function to trim whitespace from both the beginning and end of a string */
void trim_whitespace(char* str) 
{
    char *start = str;
    char *end = str + strlen(str) - 1;

    /* Trim leading whitespace */
    while (isspace((unsigned char)*start)) start++;

    /* Trim trailing whitespace */
    while (end > start && isspace((unsigned char)*end)) end--;

    /* Null-terminate the string after the last non-whitespace character */
    *(end + 1) = '\0';

     /* Copy the trimmed string back to the start if necessary */
        if (start != str) 
    	{
           memmove(str, start, end - start + 2); /* +1 for null-terminator, +1 for 0-based index */
        }
}






