# CS241 Complete Compiler Pipeline

## Overview
This repository contains a complete compiler implementation for the WLP4 language, a C-like programming language that compiles to MIPS assembly. The compiler is built as a series of assignments that progressively build up the complete compilation pipeline.

## WLP4 Language
WLP4 is a simplified C-like language with the following features:
- Integer data types and pointers
- Functions with parameters and return values
- Control flow (if-else, while loops)
- Memory management (new/delete)
- Basic I/O operations

## Compiler Pipeline

The complete compilation process consists of the following phases:

```
WLP4 Source Code
       ↓
   [Assignment 4] Lexical Analysis (Scanner/DFA)
       ↓
   Token Stream
       ↓
   [Assignment 6] Syntax Analysis (LR(1) Parser)
       ↓
   Parse Tree (AST)
       ↓
   [Assignment 7] Code Generation (wlp4gen)
       ↓
   MIPS Assembly Code
       ↓
   [Assignment 3] Assembly to Binary (Assembler)
       ↓
   MIPS Binary Code
       ↓
   [Assignment 8] Linking and Runtime
       ↓
   Executable MIPS Program
```

## Assignment Structure

### Assignment 1: MIPS Assembly Programming
- **Purpose**: Learn MIPS assembly language fundamentals
- **Components**: Basic arithmetic, memory operations, control flow
- **Output**: MIPS assembly programs
- **Pipeline Role**: Target language for code generation

### Assignment 2: MIPS Assembly Functions and Procedures
- **Purpose**: Advanced MIPS programming with functions
- **Components**: Function calls, stack management, parameter passing
- **Output**: Complex MIPS assembly with function support
- **Pipeline Role**: Function call mechanisms for code generation

### Assignment 3: MIPS Assembly to Binary Translation
- **Purpose**: Implement an assembler
- **Components**: Lexical analysis, symbol table, instruction encoding
- **Output**: Binary machine code from assembly
- **Pipeline Role**: Final code generation phase

### Assignment 4: Lexical Analysis (Scanner/DFA)
- **Purpose**: Tokenize source code
- **Components**: DFA implementation, token recognition
- **Output**: Stream of tokens
- **Pipeline Role**: First compilation phase

### Assignment 5: Syntax Analysis (Parser)
- **Purpose**: Basic parsing with top-down approach
- **Components**: Grammar processing, parse tree construction
- **Output**: Parse tree (AST)
- **Pipeline Role**: Syntax analysis foundation

### Assignment 6: LR(1) Parser Implementation
- **Purpose**: Advanced bottom-up parsing
- **Components**: LR(1) parsing, action/goto tables
- **Output**: Complete parse tree for WLP4
- **Pipeline Role**: Enhanced syntax analysis

### Assignment 7: Code Generation (wlp4gen)
- **Purpose**: Generate MIPS assembly from parse tree
- **Components**: AST traversal, register allocation, code templates
- **Output**: MIPS assembly code
- **Pipeline Role**: High-level to low-level translation

### Assignment 8: Complete Compiler Pipeline
- **Purpose**: Integrate all components
- **Components**: Full compilation pipeline, error handling
- **Output**: Complete WLP4 compiler
- **Pipeline Role**: End-to-end compilation

### Assignment 10: Advanced Compiler Features
- **Purpose**: Add optimizations and advanced features
- **Components**: Code optimization, performance improvements
- **Output**: Enhanced compiler with optimizations
- **Pipeline Role**: Production-ready compiler

## Quick Start

### Compiling a WLP4 Program
```bash
# Simple compilation
wlp4c < input.wlp4 > output.mips

# Step-by-step compilation
cat input.wlp4 | wlp4scan | wlp4parse | wlp4gen > output.asm
cs241.linkasm < output.asm > output.merl
cs241.linker output.merl print.merl alloc.merl > linked.merl
cs241.merl 0 < linked.merl > output.mips
```

### Running a Compiled Program
```bash
# Execute on MIPS simulator
mips.twoints output.mips
```

## Example WLP4 Program
```c
int wain(int a, int b) {
    int result = 0;
    int i = 0;
    
    while (i < a) {
        result = result + b;
        i = i + 1;
    }
    
    return result;
}
```

## Dependencies
- CS241 MIPS assembler and simulator
- C++ compiler (g++)
- Make build system
- Understanding of compiler theory

## Project Structure
```
CS241-Complete-Compiler/
├── assignment1/          # MIPS Assembly Programming
├── assignment2/          # MIPS Functions and Procedures
├── assignment3/          # Assembly to Binary Translation
├── assignment4/          # Lexical Analysis (Scanner/DFA)
├── assignment5/          # Syntax Analysis (Parser)
├── assignment6/          # LR(1) Parser Implementation
├── assignment7/          # Code Generation (wlp4gen)
├── assignment8/          # Complete Compiler Pipeline
├── assignment10/         # Advanced Compiler Features
├── Cargo.toml           # Rust project configuration
└── README.md            # This file
```

## Learning Objectives
By completing this compiler project, students will understand:
- Lexical analysis and finite automata
- Syntax analysis and parsing algorithms
- Code generation techniques
- Assembly language programming
- Compiler optimization
- Software engineering principles

## Contributing
This is an educational project. Each assignment builds upon the previous ones, so they should be completed in order.

## License
Educational use only - CS241 course materials.

