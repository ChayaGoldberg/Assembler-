#ifndef GENERAL_FUNCTION_H
#define GENERAL_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned short uint16_t;

/* Types of errors for the error handling system */
typedef enum {
    ERROR_NOT_INSTRUCTION,             /**< Error indicating the instruction is not valid */
    ERROR_DUPLICATE_SYMBOL,            /**< Error indicating a symbol is defined more than once */
    ERROR_INVALID_OPERATION,           /**< Error indicating an invalid operation code */
    ERROR_MISSING_OPERAND,             /**< Error indicating a missing operand */
    ERROR_INVALID_OPERAND_TYPE,        /**< Error indicating an operand type is invalid */
    ERROR_TOO_MANY_OPERANDS,           /**< Error indicating there are too many operands for an operation */
    ERROR_INVALID_REGISTER,            /**< Error indicating an invalid register number */
    ERROR_INVALID_MACRO_NAME,          /**< Error indicating an invalid macro name */
    ERROR_EXTRA_TEXT_AFTER_ENDMACR,   /**< Error indicating extra text after the end of a macro definition */
    ERROR_EXTRA_TEXT_AFTER_MACRO,     /**< Error indicating extra text after a macro definition */
    ERROR_MACRO_ALREADY_EXISTS,        /**< Error indicating that a macro is already defined */
    ERROR_LINE_TOO_LONG,               /**< Error indicating that a line of code exceeds the maximum allowed length */
    ERROR_NO_SOURCE_OPERAND_ALLOWED,   /**< Error indicating no source operand is allowed for the given operation */
    ERROR_NO_DEST_OPERAND_ALLOWED,     /**< Error indicating no destination operand is allowed for the given operation */
    ERROR_INVALID_ADDRESSING_MODE,     /**< Error indicating an invalid addressing mode */
    ERROR_MISSING_COMMA_BETWEEN_OPERANDS, /**< Error indicating a missing comma between operands */
    ERROR_EXTRA_TEXT_AFTER_OPERANDS,  /**< Error indicating extra text after the operands */
    ERROR_EXTRA_TEXT_AFTER_COMMAND,   /**< Error indicating extra text after the command */
    ERROR_MISSING_DEST_OPERAND,        /**< Error indicating a missing destination operand */
    ERROR_MISSING_SOURCE_OPERAND,      /**< Error indicating a missing source operand */
    ERROR_COMMA_BEFORE_OPERAND,        /**< Error indicating an illegal comma before the first operand */
    ERROR_DUPLICATE_COMMA,             /**< Error indicating a duplicate comma in the instruction */
    ERROR_COMMA_AFTER_LAST_OPERAND,    /**< Error indicating an extra comma after the last operand */
    ERROR_OUT_OF_RANGE,                /**< Error indicating a value is out of the allowable range */
    /* Errors related to labels */
    ERROR_NOT_VALIED_FORMAT_FOR_LABEL,/**< Error indicating a label name format is invalid */
    ERROR_MISSING_INSTRUCTION,         /**< Error indicating a missing instruction */
    ERROR_LABEL_TO_LONG,               /**< Error indicating a label name is too long */
    ERROR_NOT_VALIED_NUM,              /**< Error indicating an invalid number format */
    ERROR_NO_MEMORY_LABEL,             /**< Error indicating failure to allocate memory for a label */
    ERROR_NAME_OF_LABEL,               /**< Error indicating the name of the label is invalid */
    ERROR_NAME_EXSISTS,                /**< Error indicating the label name already exists */
    /* Errors related to data processing */
    ERROR_MISSING_BEGINNING_QUOTE,     /**< Error indicating a missing starting double quote in a string */
    ERROR_MISSING_END_QUOTE,           /**< Error indicating a missing ending double quote in a string */
    ERROR_MISSING_DOUBLE_QUOTES,       /**< Error indicating missing double quotes */
    ERROR_LINE_TOO_SHORT,              /**< Error indicating a line of code is too short */
    ERROR_NO_CHARS,                    /**< Error indicating no characters follow the data type declaration */
    ERROR_INVALID_FORMAT,              /**< Error indicating an invalid format for data */
    ERROR_NOT_A_NUMBER,                /**< Error indicating a value that should be a number is not a valid number */
    ERROR_CONSECUTIVE_COMMAS,          /**< Error indicating consecutive commas in data */
    ERROR_MISSING_COMMA,               /**< Error indicating a missing comma between data values */
    ERROR_TRAILING_COMMA,              /**< Error indicating an extra comma at the end of the data */
    ERROR_LEADING_COMMA,               /**< Error indicating a comma at the beginning of the data */
    ERROR_NO_NUMBER_AFTER_DATA,        /**< Error indicating no number follows data type */
    /* Errors related to .entry directives */
    ERROR_NO_LABEL_AFTER_ENTRY,        /**< Error indicating a missing label after an .entry directive */
    ERROR_MORE_THEN_1_WORD_AFTER_ENTRY,/**< Error indicating more than one word follows an .entry directive */
    /* Errors related to .extern directives */
    ERROR_MORE_THEN_1_WORD_AFTER_EXTERN, /**< Error indicating more than one word follows an .extern directive */
    ERROR_NO_LABEL_AFTER_EXTERN,       /**< Error indicating a missing label after an .extern directive */
    /* Errors related to the second pass */
    ERROR_EXTERN_LABEL_WAS_DEFINED,    /**< Error indicating an external label was defined in the file */
    ERROR_ENTRY_NOT_DEFINED,           /**< Error indicating an entry label was not defined in the file */
    ERROR_UNDEFINED_LABEL,             /**< Error indicating a label was used but not defined */
    ERROR_NOT_ENOUGH_MOMMORY           /**< Error indicating insufficient memory space */
} ErrorType;

/* Boolean type definition */
typedef enum 
{
    FALSE = 0, /**< Represents the boolean false value */
    TRUE = 1   /**< Represents the boolean true value */
} Bool;

extern const char* errorMessages[]; /**< Array of error messages for different error types */

/* Prints an error message based on the error type, line number, and file name */
void printError(ErrorType errorType, int lineNumber, char* fileName);

/* Extracts and returns the first word from a given line */
char* getFirstWord(const char* line);

/* Extracts and returns the remaining line after the first word */
char* getRemainingLine(const char* line);

/* Removes the first word from a line and returns the modified line */
char* removeFirstWord(char* line);

/* Trims trailing whitespace from a string */
void trim_whitespace_end(char* str);

/* Trims both leading and trailing whitespace from a string */
void trim_whitespace(char* str);

/* Trims leading whitespace from a string */
void trim_whitespace_start(char* str);

/* Duplicates a string and returns a newly allocated copy */
char* my_strdup(const char* str);

/* Converts an integer to a 15-bit two's complement binary representation */
uint16_t to_15bit_binary(int number);

/* Prints a binary representation of an unsigned integer with a specified number of bits */
void printBinary(unsigned int value, int bits);

/* Formats a string with 2 parameters and stores the result in a buffer */
void my_snprintf(char *buffer, size_t size, const char *format, const char *param1, const char *param2);

#endif

