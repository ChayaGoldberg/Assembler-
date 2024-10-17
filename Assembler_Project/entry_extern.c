#include "general_functions.h"
#include "first_pass.h"
#include "entry_extern.h"

/* 
 * Initialize the EntryList
 * 
 * This function initializes an EntryList by setting its capacity to the 
 * initial size, its size to 0, and allocates memory for the entries array.
 * 
 * Parameters:
 *    - list: A pointer to the EntryList structure to be initialized.
 */
void initEntryList(EntryList* list) {
    list->capacity = INITIAL_SIZE;
    list->size = 0;
    list->entries = (Entry*)malloc(list->capacity * sizeof(Entry));
    
    if (list->entries == NULL) {
        fprintf(stderr, "Unable to allocate memory for EntryList\n");
        exit(EXIT_FAILURE);
    }
}

/* 
 * Add an entry to the EntryList
 * 
 * This function adds a new entry to the EntryList. If necessary, the list 
 * is resized to accommodate the new entry. The entry consists of a label 
 * name and a line number.
 * 
 * Parameters:
 *    - list: A pointer to the EntryList structure to which the entry will be added.
 *    - labelName: The label name of the entry to be added.
 *    - lineNumber: The line number associated with the entry.
 */
void addEntry(EntryList* list, const char* labelName, int lineNumber) {
    /* Resize if necessary */
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->entries = (Entry*)realloc(list->entries, list->capacity * sizeof(Entry));
        
        if (list->entries == NULL) {
            fprintf(stderr, "Unable to reallocate memory for EntryList\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Add new entry */
    list->entries[list->size].labelName = my_strdup(labelName);
    list->entries[list->size].lineNumber = lineNumber;
    list->size++;
}

/* 
 * Free the EntryList
 * 
 * This function frees the memory allocated for the EntryList, including 
 * all the label names stored in the list.
 * 
 * Parameters:
 *    - list: A pointer to the EntryList structure to be freed.
 */
void freeEntryList(EntryList* list) {
    int i;
    
    for (i = 0; i < list->size; i++) {
        free(list->entries[i].labelName);
    }
    
    free(list->entries);
}

/* 
 * Initialize the ExternList
 * 
 * This function initializes an ExternList by setting its capacity to 10, 
 * its size to 0, and allocates memory for the externs array.
 * 
 * Parameters:
 *    - list: A pointer to the ExternList structure to be initialized.
 */
void initExternList(ExternList* list) {
    list->capacity = 10; 
    list->size = 0;      
    list->externs = (Extern*)malloc(list->capacity * sizeof(Extern));
    
    if (list->externs == NULL) {
        fprintf(stderr, "Unable to allocate memory for ExternList\n");
        exit(EXIT_FAILURE);
    }
}

/* 
 * Add an extern to the ExternList
 * 
 * This function adds a new extern to the ExternList. If necessary, the list 
 * is resized to accommodate the new extern. The extern consists of a label 
 * name and a line number.
 * 
 * Parameters:
 *    - list: A pointer to the ExternList structure to which the extern will be added.
 *    - labelName: The label name of the extern to be added.
 *    - lineNumber: The line number associated with the extern.
 */
void addExtern(ExternList* list, const char* labelName, int lineNumber) {
    /* Resize if necessary */
    if (list->size >= list->capacity) {
        list->capacity *= 2; 
        list->externs = (Extern*)realloc(list->externs, list->capacity * sizeof(Extern));
        
        if (list->externs == NULL) {
            fprintf(stderr, "Unable to reallocate memory for ExternList\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Add new extern */
    list->externs[list->size].labelName = my_strdup(labelName); 
    
    if (list->externs[list->size].labelName == NULL) {
        fprintf(stderr, "Unable to allocate memory for labelName\n");
        exit(EXIT_FAILURE);
    }
    
    list->externs[list->size].lineNumber = lineNumber;
    list->size++;
}

/* 
 * Free the ExternList
 * 
 * This function frees the memory allocated for the ExternList, including 
 * all the label names stored in the list. It also sets the list's size and 
 * capacity to 0.
 * 
 * Parameters:
 *    - list: A pointer to the ExternList structure to be freed.
 */
void freeExternList(ExternList* list) {
    int i;
    
    for (i = 0; i < list->size; i++) {
        free(list->externs[i].labelName); 
    }
    
    free(list->externs); 
    list->externs = NULL; 
    list->size = 0;       
    list->capacity = 0;   
}

