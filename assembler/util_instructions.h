#ifndef UTIL_INSTRUCTIONS_H
#define UTIL_INSTRUCTIONS_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"
#include "pre_assembler.h"
#include "instructions.h"

/* 
 * Structure to represent a node in a linked list of lines.
 * - line_number: The line number in the source file.
 * - ic: The instruction counter.
 * - line_text: The text of the line.
 * - next: Pointer to the next node in the list.
 */
typedef struct Node {
    int line_number;
    int ic;
    char* line_text;
    struct Node* next; 
} Node;

/* 
 * Structure to represent an encoded instruction.
 * - opcode: 4 bits for the operation code.
 * - srcOperand: 4 bits for the source operand.
 * - destOperand: 4 bits for the destination operand.
 * - ARE: 3 bits for the ARE (Addressing, Register, or Immediate).
 */
typedef struct {
    unsigned int opcode : 4;      /* 4 bits for operation code */
    unsigned int srcOperand : 4;  /* 4 bits for source operand */
    unsigned int destOperand : 4; /* 4 bits for destination operand */
    unsigned int ARE : 3;         /* 3 bits for ARE */
} EncodedInstruction;

/* 
 * Structure to represent a simple instruction.
 * - ARE: 3 bits for the ARE.
 * - destOperand: 3 bits for the destination operand (0-7).
 * - srcOperand: 3 bits for the source operand (0-7).
 * - reserved: 6 reserved bits to complete 15 bits in total.
 */
typedef struct {
    unsigned int ARE : 3;        /* 3 bits for ARE */
    unsigned int destOperand : 3; /* 3 bits for destination operand (0-7) */
    unsigned int srcOperand : 3;  /* 3 bits for source operand (0-7) */
    unsigned int reserved : 6;    /* 6 reserved bits to complete 15 bits in total */
} SimpleInstruction;

/* 
 * Structure to represent a raw instruction.
 * - ARE: 3 bits for the ARE.
 * - num: 12 bits for the number or address of the label.
 */
typedef struct {
    unsigned int ARE : 3;        /* 3 bits for ARE */
    unsigned int num : 12;       /* 12 bits for number or address of label */
} RawInstruction;

/* 
 * Structure to represent an instruction which can be detailed, simple, or raw.
 * - instruction: Union of detailed, simple, and raw instruction structures.
 * - isRaw: Flag indicating whether the instruction is of raw type.
 * - isSimple: Flag indicating whether the instruction is of simple type.
 */
typedef struct {
    union {
        EncodedInstruction detailed; /* Detailed structure with fields like opcode, srcOperand, etc. */
        SimpleInstruction simple;    /* Simple structure with 3 bits for each field */
        RawInstruction raw;          /* Raw structure with ARE and number fields */
    } instruction;
    int isRaw;      /* Flag indicating if the instruction is of raw type (15-bit value) */
    int isSimple;   /* Flag indicating if the instruction is of simple type */
} Instruction;

/* 
 * Structure to represent an array of instructions.
 * - instructions: Pointer to an array of Instruction structures.
 * - size: Current number of instructions in the array.
 * - capacity: Allocated capacity for the instructions array.
 */
typedef struct {
    Instruction *instructions;
    size_t size;
    size_t capacity;
} InstructionArray;

/* 
 * Enumeration to specify the type of operation.
 * - NO_OPERANDS: Operation with no operands.
 * - ONE_OPERAND: Operation with one operand.
 * - TWO_OPERANDS: Operation with two operands.
 */
typedef enum {
    NO_OPERANDS,
    ONE_OPERAND,
    TWO_OPERANDS
} OperationType;

/* 
 * Structure to hold operation information.
 * - name: Name of the operation.
 * - type: Type of operation (NO_OPERANDS, ONE_OPERAND, TWO_OPERANDS).
 * - destMethods: Valid destination addressing methods.
 * - sourceMethods: Valid source addressing methods.
 * - code: Numeric code for the operation.
 */
typedef struct {
    char *name;                /* Name of the operation */
    OperationType type;        /* Type of operation: NO_OPERANDS, ONE_OPERAND, TWO_OPERANDS */
    int destMethods[5];       /* Valid destination addressing methods */
    int sourceMethods[5];     /* Valid source addressing methods */
    int code;                  /* Numeric code for the operation */
} Operation;

/* 
 * Checks if the given addressing method is valid.
 * 
 * @param method: The addressing method to check.
 * @param valid_methods: Array of valid methods.
 * @return: 1 if the method is valid, 0 otherwise.
 */
int is_valid_method(int method, int valid_methods[]);

/* 
 * Determines the addressing method for a given operand and retrieves any additional value.
 * 
 * @param operand: The operand to analyze.
 * @param extraValue: Pointer to store additional value if applicable.
 * @return: The addressing method of the operand.
 */
int get_addressing_method(char *operand, int *extraValue);

/* 
 * Initializes an InstructionArray with a specified initial capacity.
 * 
 * @param array: Pointer to the InstructionArray to initialize.
 * @param initial_capacity: The initial capacity of the array.
 */
void init_instruction_array(InstructionArray *array, size_t initial_capacity);

/* 
 * Adds a detailed (encoded) instruction to the InstructionArray.
 * 
 * @param array: Pointer to the InstructionArray.
 * @param instr: The EncodedInstruction to add.
 */
void add_detailed_instruction(InstructionArray *array, EncodedInstruction instr);

/* 
 * Frees all memory allocated for an InstructionArray.
 * 
 * @param array: Pointer to the InstructionArray to free.
 */
void free_instruction_array(InstructionArray *array);

/* 
 * Prints the details of an Instruction.
 * 
 * @param inst: Pointer to the Instruction to print.
 */
void print_instruction(Instruction *inst);

/* 
 * Prints the binary representation of a 16-bit value.
 * 
 * @param value: The 16-bit value to print.
 */
void print_binary(uint16_t value);

/* 
 * Validates if the given label is correct according to operation rules.
 * 
 * @param label: The label to check.
 * @param all_operations: Pointer to an array of all operations for validation.
 * @return: TRUE if the label is valid, FALSE otherwise.
 */
Bool is_valid_label(char *label, Operation* all_operations);

/* 
 * Appends a new node to the end of a linked list.
 * 
 * @param head: Pointer to the head of the linked list.
 * @param line_number: Line number to store in the new node.
 * @param line_text: Text of the line to store in the new node.
 * @param ic: Instruction counter to store in the new node.
 */
void append_node(Node** head, int line_number, const char* line_text, int ic);

/* 
 * Prints all nodes in a linked list.
 * 
 * @param head: Pointer to the head of the linked list.
 */
void print_list(Node* head);

/* 
 * Frees all nodes in a linked list.
 * 
 * @param head: Pointer to the head of the linked list.
 */
void free_list(Node* head);

/* 
 * Prints the content of an InstructionArray.
 * 
 * @param array: Pointer to the InstructionArray to print.
 */
void printInstructionArray(const InstructionArray *array);

/* 
 * Adds a simple instruction to the InstructionArray.
 * 
 * @param array: Pointer to the InstructionArray.
 * @param simpleInstruction: The SimpleInstruction to add.
 */
void addSimpleInstruction(InstructionArray *array, SimpleInstruction simpleInstruction);

/* 
 * Adds a raw instruction to the InstructionArray.
 * 
 * @param array: Pointer to the InstructionArray.
 * @param rawInstruction: The RawInstruction to add.
 */
void addRawInstruction(InstructionArray *array, RawInstruction rawInstruction);

#endif /* UTIL_INSTRUCTIONS_H */

