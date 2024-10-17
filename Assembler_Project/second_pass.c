#include "second_pass.h"




/*If the command type matches a label, the line number is incremented by ic.*/
void preSecondPass(LabelNode *labels, int ic) {
    LabelNode *currentNode = labels;  /* Start from the head of the linked list */

    /* Traverse the linked list */
    while (currentNode != NULL) {
        /* Check if the command type following the label is of type LABEL */
        if (currentNode->label->followingContent == LABEL) {
            /* Update the line number by adding the instruction counter (ic) */
            currentNode->label->lineNumber += ic;
        }

        /* Move to the next node in the linked list */
        currentNode = currentNode->next;
    }
}




/* 
 * Checks if a label name exists in the label list and returns its line number.
 */
int check_label_name(LabelNode* labels, char* label_name) 
{
    /* Pointer to traverse the label list */
    LabelNode* labels_current = labels;

    /* Traverse the list until the end */
    while (labels_current != NULL) 
    {
        /* Compare label names */
        if (strcmp(labels_current->label->name, label_name) == 0) 
        {
            /* Return the line number if label is found */
            return labels_current->label->lineNumber;
        }
        labels_current = labels_current->next;
    }
    
    /* Return 0 if label is not found */
    return 0;
}

/* 
 * Checks if a label name exists in the extern list.
 */
Bool check_label_extern_name(ExternList* externList, char* label_name) 
{
    int i;

    /* Iterate over the extern list */
    for (i = 0; i < externList->size; i++) 
    {
        /* Compare label names */
        if (strcmp(externList->externs[i].labelName, label_name) == 0) 
        {
            /* Return TRUE if label is found in extern list */
            return TRUE;  
        }
    }
    
    /* Return FALSE if label is not found */
    return FALSE;  
}

/* 
 * Processes labels and updates instruction array based on label usage.
 */
void check_if_label(LabelNode* labels, Node* label_list_used, InstructionArray* instructionArray, Bool* no_errors, char *file_name, ExternList externList, FILE* ext_file, Bool* is_extern) 
{
    int num;
    int current_IC;
    Instruction* array_list;
    Node *current = label_list_used;

    /* Iterate over the labels used in instructions */
    while (current != NULL)
    {
        array_list = instructionArray->instructions;
        num = check_label_name(labels, current->line_text);

        /* Check if label is defined */
        if (num != 0)
        {
            current_IC = current->ic - 100;
            /* Traverse the instruction array to find the correct position */
            while (current_IC != 0 && array_list != NULL)
            {
                array_list++;
                current_IC--;
            }
            /* Update instruction if label is found */
            if (array_list != NULL)
            {
                if (array_list->isRaw)
                {
                    array_list->instruction.raw.ARE = 2;
                    array_list->instruction.raw.num = num;
                }
            }
        }
        /* Check if label is extern */
        else if (check_label_extern_name(&externList, current->line_text))
        {
            current_IC = current->ic - 100;
            *is_extern = FALSE;
            /* Traverse the instruction array to find the correct position */
            while (current_IC != 0 && array_list != NULL)
            {
                array_list++;
                current_IC--;
            }
            /* Update instruction and write to extern file */
            if (array_list != NULL)
            {
                array_list->instruction.raw.ARE = 1;
                fprintf(ext_file, "%s %04d\n", current->line_text, current->ic);
            }
        }
        else
        {
            /* Report error if label is undefined */
            printError(ERROR_UNDEFINED_LABEL, current->line_number, file_name);
            *no_errors = FALSE;
        }
        current = current->next;
    }
}

/* 
 * Checks for illegal extern labels by comparing them against the label list.
 */
void check_alligal_extern_labels(LabelNode** labels, ExternList* externList, Bool* no_errors, char *file_name)
{
    int i;
    ExternList* current_externList = externList;

    /* Iterate over the extern list */
    for (i = 0; i < externList->size; i++) 
    {
        /* Check if extern label is also defined in the label list */
        if (check_label_name(*labels, current_externList->externs[i].labelName) != 0) 
        {
            /* Report error for illegal extern label */
            printError(ERROR_EXTERN_LABEL_WAS_DEFINED, current_externList->externs[i].lineNumber, file_name);
            *no_errors = FALSE;
        }
    }
}

	

/* 
 * Checks all entry labels for validity and writes valid entries to a file.
 */
void check_alligal_entry_labels(LabelNode** labels, Bool* no_errors, char *file_name, EntryList** entryList, FILE* file_ent)
{
    /* Pointer to the current entry list */
    EntryList* current_entryList = *entryList;
    int num;
    int i;

    /* Iterate over each entry label in the entry list */
    for (i = 0; i < current_entryList->size; i++) 
    {
        /* Check if the entry label exists in the label list */
        num = check_label_name(*labels, current_entryList->entries[i].labelName);

        /* If the label is not found, report an error */
        if (num == 0)
        {
            *no_errors = FALSE;
            printError(ERROR_ENTRY_NOT_DEFINED, current_entryList->entries[i].lineNumber, file_name);
        }
        else
        {
            /* If the label is valid, write the entry to the file */
            fprintf(file_ent, "%s %04d\n", current_entryList->entries[i].labelName, num);
        }
    }    
}

	
/* 
 * Processes the second pass of the assembler, opens output files, performs necessary checks,
 * writes results to files, and frees allocated memory.
 * 
 */
void secondPass(char *file_name, int* IC, int* DC, LabelNode** labels, ExternList* externList, EntryList** entryList, Data** head, CharData** headChar, InstructionArray* instructionArray, Node** label_list_used, Bool* no_errors)
{
    FILE *file_ob, *file_ent, *file_ext;  /* File pointers for writing */
    int length = strlen(file_name);  /* Length of the file name without extension */
    Bool is_extern = TRUE;

    /* Allocate memory for file names with enough space for extensions and NULL terminator */
    char *ob_filename = malloc(length + 4);  /* +4 for ".ob\0" */
    char *ent_filename = malloc(length + 5); /* +5 for ".ent\0" */
    char *ext_filename = malloc(length + 5); /* +5 for ".ext\0" */
    /* Check if memory allocation was successful */
    if (ob_filename == NULL || ent_filename == NULL || ext_filename == NULL) 
    {
        fprintf(stderr, "Unable to allocate memory for file names\n");
        exit(EXIT_FAILURE);
    }

    /* Build file names with the appropriate extensions */
    my_snprintf(ob_filename, length + 4, "%s.ob", file_name, ".ob");
    my_snprintf(ent_filename, length + 5, "%s.ent", file_name, ".ent");
    my_snprintf(ext_filename, length + 5, "%s.ext", file_name, ".ext");

    /* Open .ob file for writing */
    file_ob = fopen(ob_filename, "w");
    if (file_ob == NULL) 
    {
        fprintf(stderr, "Error opening .ob file: %s\n", ob_filename);
        free(ob_filename);  /* Free allocated memory before exiting */
        free(ent_filename);
        free(ext_filename);
        exit(EXIT_FAILURE);
    }

    /* Open .ent file for writing */
    file_ent = fopen(ent_filename, "w");
    if (file_ent == NULL) 
    {
        fprintf(stderr, "Error opening .ent file: %s\n", ent_filename);
        fclose(file_ob);  /* Close previously opened files */
        free(ob_filename);
        free(ent_filename);
        free(ext_filename);
        exit(EXIT_FAILURE);
    }

    /* Open .ext file for writing */
    file_ext = fopen(ext_filename, "w");
    if (file_ext == NULL) 
    {
        fprintf(stderr, "Error opening .ext file: %s\n", ext_filename);
        fclose(file_ob);  /* Close previously opened files */
        fclose(file_ent);
        free(ob_filename);
        free(ent_filename);
        free(ext_filename);
        exit(EXIT_FAILURE);
    }

    /* Perform the second pass operations */
    preSecondPass(*labels, *IC);
    check_alligal_extern_labels(labels, externList, no_errors, file_name);
    check_alligal_entry_labels(labels, no_errors, file_name, entryList, file_ent);
    check_if_label(*labels, *label_list_used, instructionArray, no_errors, file_name, *externList, file_ext, &is_extern);

    /* Write instructions and data to .ob file */
    fprintf(file_ob, "   %d  %d\n", *IC - 100, *DC);
    printInstructionsInOctal(instructionArray, file_ob);
    printDataAndCharData(head, headChar, IC, file_ob);

    /* Free allocated memory */
    free_list(*label_list_used);
    free_instruction_array(instructionArray);
    freeLabels(*labels);
    freeEntryList(*entryList);
    freeExternList(externList);
    freeDataList(*head);
    freeCharDataList(*headChar);

    /* Handle errors and clean up */
    if (*no_errors == FALSE)
    {
        remove(ob_filename);
        remove(ent_filename);
        remove(ext_filename);
    }
    /*if there is no room in mommory*/
    if (*IC > MAX_MOMMORY)
    {
        printError(ERROR_NOT_ENOUGH_MOMMORY, MAX_MOMMORY, file_name);
        remove(ob_filename);
        remove(ent_filename);
        remove(ext_filename);
    }
    else
    {
        fclose(file_ent);
        fclose(file_ext);
        if ((*entryList)->size == 0) 
        {
            remove(ent_filename);
        }
        if (is_extern)
        {
            remove(ext_filename);
        }
        fclose(file_ob);
    }

    /* Free allocated memory */
    free(ob_filename);
    free(ent_filename);
    free(ext_filename);
}

/* 
 * Prints the instructions in octal format to the specified file.
 * 
 */
void printInstructionsInOctal(InstructionArray *instructionArray, FILE *file_ob) 
{
    int ic = 100;
    int i;
    for (i = 0; i < instructionArray->size; i++) 
    {
        unsigned int binaryValue = 0;
        if (instructionArray->instructions[i].isRaw) 
        {
            /* Generate binary value for RawInstruction */
            binaryValue = (instructionArray->instructions[i].instruction.raw.ARE) |
                          (instructionArray->instructions[i].instruction.raw.num << 3);
        } 
        else if (instructionArray->instructions[i].isSimple) 
        {
            /* Generate binary value for SimpleInstruction */
            binaryValue = (instructionArray->instructions[i].instruction.simple.ARE) |
                          (instructionArray->instructions[i].instruction.simple.srcOperand << 6) |
                          (instructionArray->instructions[i].instruction.simple.destOperand << 3);
        } 
        else 
        {
            /* Generate binary value for EncodedInstruction */
            binaryValue = (instructionArray->instructions[i].instruction.detailed.ARE) |
                          (instructionArray->instructions[i].instruction.detailed.srcOperand << 7) |
                          (instructionArray->instructions[i].instruction.detailed.destOperand << 3) |
                          (instructionArray->instructions[i].instruction.detailed.opcode << 11);
        }

        /* Print the value in octal format */
        fprintf(file_ob, "%04d %05o\n", ic, binaryValue);  /* %05o ensures 5 octal digits, even if leading zeros */
        ic++;
    }
}

/* 
 * Prints data and character data values to the specified file.
 * 
 */
void printDataAndCharData(Data** head, CharData** headChar, int* IC, FILE* file_ob) 
{
    Data* currentData = *head;
    CharData* currentChar = *headChar;
    while (currentData != NULL || currentChar != NULL) 
    {
        if (currentData != NULL && (currentChar == NULL || currentData->lineNumber <= currentChar->lineNumber)) 
        {
            fprintf(file_ob, "%04d %05o\n", *IC, currentData->numberValue);
            currentData = currentData->next;
            (*IC)++;
        } 
        else if (currentChar != NULL && (currentData == NULL || currentChar->lineNumber < currentData->lineNumber)) 
        {
            fprintf(file_ob, "%04d %05o\n", *IC, currentChar->asciiValue);
            currentChar = currentChar->next;
            (*IC)++;
        }
    }
}

