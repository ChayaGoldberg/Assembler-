#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>     /* Standard input/output definitions */
#include <stdlib.h>    /* Standard library definitions */
#include <string.h>    /* String manipulation functions */

#include "data.h"           /* Data structures and functions */
#include "entry_extern.h"   /* Entry and external symbols handling */
#include "general_functions.h"  /* General utility functions */
#include "instructions.h"   /* Instruction-related definitions */

/* Define maximum length for label names */
#define LABEL_MAX_LENGTH 32

/* Enum to represent different command types that can follow a label */
typedef enum {
    LABEL,       /* Indicates a label */
    INSTRUCTION, /* Indicates an instruction following the label */
    ERROR        /* Indicates an error or invalid type */
} CommandType;

/* Structure to represent a label */
typedef struct Label {
    char name[LABEL_MAX_LENGTH];   /* Name of the label */
    int lineNumber;                /* Line number where the label is defined */
    CommandType followingContent;  /* Type of content that follows the label */
} Label;

/* Structure for a node in a linked list of labels */
typedef struct LabelNode {
    Label* label;                /* Pointer to the label structure */
    struct LabelNode* next;      /* Pointer to the next node in the list */
} LabelNode;

/**
 * Creates a new Label structure.
 *
 * @param name: The name of the label.
 * @param lineNumber: The line number where the label is defined.
 * @param followingContent: The type of content following the label.
 *
 * @return: A pointer to the newly created Label structure.
 */
Label* createLabel(const char* name, int lineNumber, CommandType followingContent);

/**
 * Adds a label to the linked list of labels.
 *
 * @param head: Pointer to the head of the linked list.
 * @param newLabel: Pointer to the new Label structure to be added.
 */
void addLabel(LabelNode** head, Label* newLabel);

/**
 * Checks if a label is valid based on its token, line number, and file name.
 *
 * @param token: The label token to be checked.
 * @param lineNumber: The line number where the label is defined.
 * @param file_name: The name of the file being processed.
 *
 * @return: 1 if the label is valid, 0 otherwise.
 */
int isValidLabel(const char* token, int lineNumber, char *file_name);

/**
 * Checks if a label with the given name already exists in the linked list.
 *
 * @param labels: Pointer to the head of the linked list of labels.
 * @param name: The name of the label to be checked.
 *
 * @return: 1 if the label exists, 0 otherwise.
 */
int labelExists(LabelNode* labels, const char* name);

/**
 * Determines the type of the next word in the line based on its content.
 *
 * @param line: The line of text to be analyzed.
 * @param lineNumber: The line number being processed.
 * @param file_name: The name of the file being processed.
 * @param no_errors: Flag indicating if errors should be reported.
 *
 * @return: The type of the next word (LABEL, INSTRUCTION, or ERROR).
 */
CommandType nextWordType(const char* line, int lineNumber, char *file_name, Bool no_errors);

/**
 * Checks if a token ends with a colon ':'.
 *
 * @param token: The token to be checked.
 *
 * @return: 1 if the token ends with ':', 0 otherwise.
 */
int endsWithColon(const char* token);

/**
 * Checks if the given name corresponds to a valid command type.
 *
 * @param name: The command name to be checked.
 *
 * @return: 1 if the name is a valid command type, 0 otherwise.
 */
int correctCommand(const char *name);

/**
 * Prints the details of a single label.
 *
 * @param label: Pointer to the Label structure to be printed.
 */
void printLabel(const Label* label);

/**
 * Prints all labels in the linked list.
 *
 * @param head: Pointer to the head of the linked list of labels.
 */
void printLabelList(const LabelNode* head);

/**
 * Frees the memory allocated for the linked list of labels.
 *
 * @param head: Pointer to the head of the linked list of labels.
 */
void freeLabels(LabelNode* head);

#endif /* LABEL_H */

