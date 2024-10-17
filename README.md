# Assembler Project

## Overview

This project implements an assembler for a custom machine with a memory size of 4096 bytes and 8 registers. Each byte in memory consists of 15 bits, and the assembler translates assembly code into machine language for this custom architecture. The assembler works in two main phases:
### Requirements:
    Pass 1 (First Pass): In the first pass, the assembler processes the input file to:
        Expand and validate macros.
        Identify and validate labels.
        Parse different types of instructions.
        Store necessary data in appropriate data structures for the second pass.
    Pass 2 (Second Pass): The assembler then takes the information gathered from the first pass and:
        Translates assembly instructions and data into 15-bit machine code.
        Resolves label addresses.
        Outputs the necessary files for machine execution, including object, entry, and extern files.

### Assembly Instructions Supported:

This assembler supports various assembly commands, including:

    **Data Instructions:**
        .data - Used for declaring numerical data.
        .string - Used for defining strings stored as ASCII values.
        .extern - Declares external labels used in other files.
        .entry - Marks labels as entry points that can be accessed externally.

    **Command Instructions:**
        mov - Move data between registers or memory.
        cmp - Compare two operands.
        add, sub - Perform addition or subtraction between two operands.
        lea - Load effective address.
        clr, not, inc, dec - Clear, invert, increment, or decrement a value.
        jmp, bne, jsr - Jump to address, branch if not equal, or jump to subroutine.
        red, prn - Read from input or print data.
        rts, stop - Return from subroutine or stop the program execution.

## Output:

**The assembler generates the following output files:**

    Object File (.ob) - Contains the machine code translated from the assembly instructions.
    Entry File (.ent) - Contains entries for labels marked with the .entry directive.
    Extern File (.ext) - Contains information for labels marked with the .extern directive.

## Directory Structure
Assembler_Project/ ├── assembler/ │ ├── assembler.c │ ├── assembler.h │ ├── assembler.o │ ├── data.c │ ├── data.h │ ├── data.o │ ├── entry_extern.c │ ├── entry_extern.h │ ├── entry_extern.o │ ├── first_pass.c │ ├── first_pass.h │ ├── first_pass.o │ ├── general_functions.c │ ├── general_functions.h │ ├── general_functions.o │ ├── instructions.c │ ├── instructions.h │ ├── instructions.o │ ├── label.c │ ├── label.h │ ├── label.o │ ├── makefile │ ├── pre_assembler.c │ ├── pre_assembler.h │ ├── pre_assembler.o │ ├── second_pass.c │ ├── second_pass.h │ ├── second_pass.o │ ├── util_instructions.c │ ├── util_instructions.h │ ├── util_instructions.o │ ├── util_pre_assembler.c │ ├── util_pre_assembler.h │ └── util_pre_assembler.o ├── invalid_input/ └── valid_input/

## File Descriptions

- **assembler.c**: 
  - Contains the main function that manages the overall workflow of the assembler. It opens input files, checks their validity, and generates output files if the inputs are valid.

- **assembler.h**: 
  - Header file that contains declarations for the functions implemented in `assembler.c`.

- **data.c**: 
  - Implements functions that handle data types and storage, including managing any data commands within the assembly code.

- **data.h**: 
  - Header file containing declarations for the data functions.

- **entry_extern.c**: 
  - Manages entry and extern labels, ensuring that they are processed correctly during the assembly process.

- **entry_extern.h**: 
  - Header file containing declarations related to entry and extern label handling.

- **first_pass.c**: 
  - Contains functions that iterate over the input code, validate instructions, labels, and store the validated information in structured formats for further processing.

- **first_pass.h**: 
  - Header file containing function declarations used in the first pass of the assembly.

- **general_functions.c**: 
  - Provides utility functions that are used throughout the project, such as string manipulation or error handling.

- **general_functions.h**: 
  - Header file containing declarations for the general utility functions.

- **instructions.c**: 
  - Implements functions to handle various instruction types, including their validation and encoding.

- **instructions.h**: 
  - Header file with function declarations related to instruction processing.

- **label.c**: 
  - Manages the creation and validation of labels, ensuring that they are correctly referenced in the assembly code.

- **label.h**: 
  - Header file containing declarations for label management functions.

- **makefile**: 
  - A script for automating the build process, specifying how to compile and link the program.

- **pre_assembler.c**: 
  - Responsible for processing macros and performing the first pass of the assembly. It identifies and expands macros before the main assembly process.

- **pre_assembler.h**: 
  - Header file containing declarations for functions related to macro handling.

- **second_pass.c**: 
  - Processes the structured data generated during the first pass, converting valid instructions into their binary representations.

- **second_pass.h**: 
  - Header file containing declarations for functions used in the second pass of the assembly.

- **util_instructions.c**: 
  - Provides additional utility functions for instruction processing, such as encoding formats.

- **util_instructions.h**: 
  - Header file containing declarations for utility functions related to instructions.

- **util_pre_assembler.c**: 
  - Contains utility functions specifically for pre-assembly processes, such as macro validation and expansion.

- **util_pre_assembler.h**: 
  - Header file containing declarations for utility functions related to pre-assembly.

## Usage
**To compile the assembler, run:**

make

**To execute the assembler, use:**

./assembler [input_file]

## Contributing

Feel free to fork the repository and submit a pull request with your changes if you'd like to contribute



