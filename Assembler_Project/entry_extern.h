#ifndef ENTRY_EXTERN_H
#define ENTRY_EXTERN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_SIZE 10

/* Structure to represent an entry in the EntryList */
typedef struct {
    char* labelName;                 
    int lineNumber;                
} Entry;

/* Define the EntryList structure */
typedef struct {
    Entry* entries;     /* Array of entries */
    int size;           /* Number of entries currently in the list */
    int capacity;       /* Total capacity of the list */
} EntryList;

/* Structure to represent an external label in the ExternList */
typedef struct {
    char* labelName;                 
    int lineNumber;                
} Extern;

/* Define the ExternList structure */
typedef struct {
    Extern* externs;    /* Array of externs */
    int size;           /* Number of externs currently in the list */
    int capacity;       /* Total capacity of the list */
} ExternList;

/* Initialize the EntryList */
void initEntryList(EntryList* list);

/* Add an entry to the EntryList */
void addEntry(EntryList* list, const char* labelName, int lineNumber);

/* Free the EntryList */
void freeEntryList(EntryList* list);

/* Initialize the ExternList */
void initExternList(ExternList* list);

/* Add an extern to the ExternList */
void addExtern(ExternList* list, const char* labelName, int lineNumber);

/* Free the ExternList */
void freeExternList(ExternList* list);

#endif /* ENTRY_EXTERN_H */
