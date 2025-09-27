# Assignment 3: MIPS Assembly to Binary Translation

## Overview
This assignment implements an assembler that translates MIPS assembly code into binary machine code. This is a crucial component of the compiler pipeline as it converts the generated assembly into executable binary format.

## Components
- **asm.cc**: Main assembler implementation
- **scanner.h/scanner.cc**: Lexical analysis for assembly parsing
- **binasm**: Compiled assembler executable
- **Makefile**: Build configuration

## Key Features
- **Lexical Analysis**: Tokenizes MIPS assembly instructions
- **Symbol Table**: Manages labels and their addresses
- **Instruction Encoding**: Converts assembly to 32-bit binary instructions
- **Error Handling**: Detects syntax errors and duplicate labels
- **Binary Output**: Generates executable machine code

## Supported Instructions
- Arithmetic: `add`, `sub`, `mult`, `multu`, `div`, `divu`
- Memory: `lw`, `sw`, `lis`
- Control: `beq`, `bne`, `jr`, `jalr`
- Comparison: `slt`, `sltu`
- Special: `mfhi`, `mflo`

## Pipeline Position
Acts as the final stage of code generation, converting assembly to executable binary.

## Usage
```bash
make                    # Compile the assembler
./binasm < input.asm    # Assemble MIPS code to binary
```

## Example
Input assembly:
```mips
add $3, $1, $2
jr $31
```

Output: Binary machine code (4 bytes per instruction)

## Key Implementation Details
- **Two-pass assembly**: First pass builds symbol table, second pass generates code
- **Instruction encoding**: Uses bit manipulation to create proper MIPS instruction format
- **Label resolution**: Handles forward references and relative addressing
- **Error detection**: Validates instruction syntax and operand types

## Dependencies
- Assignment 1 & 2 (MIPS assembly knowledge)
- C++ programming skills
- Understanding of binary representation

