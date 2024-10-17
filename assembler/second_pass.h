#ifndef SECOND_PAST_H
#define SECOND_PAST_H
#define MAX_MOMMORY 4096

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"
#include "label.h"
#include "data.h"
#include "entry_extern.h"
#include "general_functions.h"
#include "instructions.h"
#include "pre_assembler.h"
#include "util_instructions.h"

/**
 * Opens the files needed for the second pass of assembly.
 * 
 * This function is responsible for opening the specified file and setting up
 * any necessary file handles or configurations required for the second pass.
 * 
 * @param file_name: The name of the file to open.
 */
void openFiles(const char *file_name);

/**
 * Performs the second pass of assembly on the provided label list and instruction count.
 * 
 * This function processes the label list and instruction count to ensure that all labels
 * and instructions are correctly handled during the second pass of assembly.
 * 
 * @param head: A pointer to the head of the label list.
 * @param ic: The instruction count to be used during the second pass.
 */
void preSecondPass(LabelNode *head, int ic);

/**
 * Checks if a label name is valid and exists in the provided label list.
 * 
 * This function verifies whether a given label name is present in the label list.
 * 
 * @param labels: A pointer to the label list.
 * @param label_name: The name of the label to check.
 * @return: Returns 1 if the label name is found, 0 otherwise.
 */
int check_label_name(LabelNode* labels, char* label_name);

/**
 * Checks for illegal extern labels and updates the error status.
 * 
 * This function verifies if there are any illegal extern labels in the provided extern list.
 * It also updates the error status and outputs any relevant errors to the specified file.
 * 
 * @param labels: A pointer to the label list.
 * @param externList: A pointer to the list of extern labels.
 * @param no_errors: A pointer to a boolean flag indicating if there are no errors.
 * @param file_name: The name of the file where errors will be logged.
 */
void check_alligal_extern_labels(LabelNode** labels, ExternList* externList, Bool* no_errors, char *file_name);

/**
 * Checks if a label name is in the extern list.
 * 
 * This function verifies whether a given label name is present in the extern list.
 * 
 * @param externList: A pointer to the list of extern labels.
 * @param label_name: The name of the label to check.
 * @return: Returns true if the label name is found in the extern list, false otherwise.
 */
Bool check_label_extern_name(ExternList* externList, char* label_name);

/**
 * Checks for illegal entry labels and updates the error status.
 * 
 * This function verifies if there are any illegal entry labels in the provided entry list.
 * It also updates the error status and outputs any relevant errors to the specified file.
 * 
 * @param labels: A pointer to the label list.
 * @param no_errors: A pointer to a boolean flag indicating if there are no errors.
 * @param file_name: The name of the file where errors will be logged.
 * @param entryList: A pointer to the list of entry labels.
 * @param file_ent: A file pointer where entry labels will be written.
 */
void check_alligal_entry_labels(LabelNode** labels, Bool* no_errors, char *file_name, EntryList** entryList, FILE* file_ent);

/**
 * Checks if labels are used correctly and updates error status.
 * 
 * This function verifies if the labels are used correctly in the provided label list
 * and instruction array. It also checks for extern labels and updates the error status.
 * 
 * @param labels: A pointer to the label list.
 * @param label_list_used: A pointer to the list of labels that have been used.
 * @param instructionArray: A pointer to the array of instructions.
 * @param no_errors: A pointer to a boolean flag indicating if there are no errors.
 * @param file_name: The name of the file where errors will be logged.
 * @param externList: A pointer to the list of extern labels.
 * @param ext_file: A file pointer where extern labels will be written.
 * @param is_extern: A pointer to a boolean flag indicating if the label is extern.
 */
void check_if_label(LabelNode* labels, Node* label_list_used, InstructionArray* instructionArray, Bool* no_errors, char *file_name, ExternList externList, FILE* ext_file, Bool* is_extern);

/**
 * Executes the second pass of assembly, processing labels, externs, and entries.
 * 
 * This function performs the second pass of assembly on the provided data, processing
 * labels, externs, entries, and instructions. It also updates the instruction and data
 * counts and handles any errors encountered.
 * 
 * @param file_name: The name of the file to process.
 * @param IC: A pointer to the instruction count.
 * @param DC: A pointer to the data count.
 * @param labels: A pointer to the label list.
 * @param externList: A pointer to the list of extern labels.
 * @param entryList: A pointer to the list of entry labels.
 * @param head: A pointer to the head of the data list.
 * @param headChar: A pointer to the head of the character data list.
 * @param instructionArray: A pointer to the array of instructions.
 * @param label_list_used: A pointer to the list of labels that have been used.
 * @param no_errors: A pointer to a boolean flag indicating if there are no errors.
 */
void secondPass(char *file_name, int* IC, int* DC, LabelNode** labels, ExternList* externList, EntryList** entryList, Data** head, CharData** headChar, InstructionArray* instructionArray, Node** label_list_used, Bool* no_errors);

/**
 * Prints the instructions in octal format to the specified file.
 * 
 * This function converts and prints the instructions in the instruction array to octal
 * format and writes them to the specified object file.
 * 
 * @param instructionArray: A pointer to the array of instructions.
 * @param file_ob: A file pointer to the object file where the instructions will be written.
 */
void printInstructionsInOctal(InstructionArray *instructionArray, FILE* file_ob);

/**
 * Prints the data and character data to the specified object file.
 * 
 * This function writes the data and character data to the specified object file,
 * including the instruction count and data count.
 * 
 * @param head: A pointer to the head of the data list.
 * @param headChar: A pointer to the head of the character data list.
 * @param IC: A pointer to the instruction count.
 * @param file_ob: A file pointer to the object file where the data will be written.
 */
void printDataAndCharData(Data** head, CharData** headChar, int* IC, FILE* file_ob);

#endif /* SECOND_PAST_H */

