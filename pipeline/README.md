# 🏗️ WLP4 to x86 Compiler Pipeline

## Overview
This directory contains the complete WLP4 to x86 compiler pipeline, organized into 7 numbered stages for clear understanding and maintenance.

## Pipeline Architecture

```
01_lexical_analysis → 02_syntax_analysis → 03_semantic_analysis → 04_code_generation → 05_assembly → 06_linking → 07_execution
         ↓                    ↓                     ↓                      ↓                ↓            ↓            ↓
    wlp4scan.cc         wlp4parse.cc           (integrated)         wlp4gen_x86.cc   x86_assembler.cpp  x86_linker.cpp  (execution)
```

## Directory Structure

```
pipeline/
├── 01_lexical_analysis/     # Stage 1: Tokenization
│   ├── wlp4scan.cc         # Scanner implementation
│   ├── samples/            # WLP4 source files
│   └── README.md           # Stage documentation
│
├── 02_syntax_analysis/      # Stage 2: Parsing
│   ├── wlp4parse.cc        # Parser implementation
│   ├── samples/            # Token files and AST outputs
│   └── README.md           # Stage documentation
│
├── 03_semantic_analysis/    # Stage 3: Semantic Analysis
│   ├── samples/            # AST files and semantic outputs
│   └── README.md           # Stage documentation
│
├── 04_code_generation/      # Stage 4: Code Generation
│   ├── wlp4gen_x86.cc      # Code generator implementation
│   ├── samples/            # AST files and assembly outputs
│   └── README.md           # Stage documentation
│
├── 05_assembly/             # Stage 5: Assembly
│   ├── x86_assembler.cpp   # Assembler implementation
│   ├── samples/            # Assembly files and object code
│   └── README.md           # Stage documentation
│
├── 06_linking/              # Stage 6: Linking
│   ├── x86_linker.cpp      # Linker implementation
│   ├── samples/            # Object files and executables
│   └── README.md           # Stage documentation
│
└── 07_execution/            # Stage 7: Execution
    ├── samples/            # Final executables
    └── README.md           # Stage documentation
```

## Stage Descriptions

### Stage 1: Lexical Analysis
- **Purpose**: Convert WLP4 source to tokens
- **Tool**: `wlp4scan.cc`
- **Input**: WLP4 source code
- **Output**: Token stream

### Stage 2: Syntax Analysis
- **Purpose**: Parse tokens into AST
- **Tool**: `wlp4parse.cc`
- **Input**: Token stream
- **Output**: Abstract Syntax Tree

### Stage 3: Semantic Analysis
- **Purpose**: Validate semantic correctness
- **Tool**: Integrated into parser/code generator
- **Input**: AST
- **Output**: Validated AST

### Stage 4: Code Generation
- **Purpose**: Generate x86 assembly from AST
- **Tool**: `wlp4gen_x86.cc`
- **Input**: AST
- **Output**: x86 assembly code

### Stage 5: Assembly
- **Purpose**: Convert assembly to object code
- **Tool**: `x86_assembler.cpp`
- **Input**: x86 assembly
- **Output**: Object file

### Stage 6: Linking
- **Purpose**: Create executable from object files
- **Tool**: `x86_linker.cpp`
- **Input**: Object files
- **Output**: Executable binary

### Stage 7: Execution
- **Purpose**: Execute the final program
- **Tool**: System loader
- **Input**: Executable binary
- **Output**: Program results

## Quick Start

### Build All Tools
```bash
# Build scanner
cd 01_lexical_analysis/
g++ -std=c++14 -Wall -o wlp4scan wlp4scan.cc

# Build parser
cd ../02_syntax_analysis/
g++ -std=c++14 -Wall -o wlp4parse wlp4parse.cc

# Build code generator
cd ../04_code_generation/
g++ -std=c++14 -Wall -o wlp4gen_x86 wlp4gen_x86.cc

# Build assembler
cd ../05_assembly/
g++ -std=c++14 -Wall -o x86_assembler x86_assembler.cpp

# Build linker
cd ../06_linking/
g++ -std=c++14 -Wall -o x86_linker x86_linker.cpp
```

### Run Complete Pipeline
```bash
# Compile a WLP4 program through all stages
cd 01_lexical_analysis/
./wlp4scan < samples/01_simple_addition.wlp4 | \
cd ../02_syntax_analysis/ && ./wlp4parse | \
cd ../04_code_generation/ && ./wlp4gen_x86 | \
cd ../05_assembly/ && ./x86_assembler /dev/stdin temp.o && \
cd ../06_linking/ && ./x86_linker final_executable ../05_assembly/temp.o
```

## Sample Programs

Each stage includes sample programs demonstrating the pipeline:
- `01_simple_addition.wlp4` - Basic arithmetic
- `02_multiplication.wlp4` - Multiplication operation
- `03_hello_world.wlp4` - Print statement

## Key Features

✅ **Clear Stage Separation**: Each stage has its own directory and purpose  
✅ **Numbered Organization**: Easy to follow the pipeline flow  
✅ **Complete Documentation**: Each stage has detailed README  
✅ **Sample Programs**: Working examples at each stage  
✅ **Modular Design**: Each stage can be built and tested independently  

This organization makes the WLP4 to x86 compiler pipeline **professional, maintainable, and easy to understand**! 🚀
