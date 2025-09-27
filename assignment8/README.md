# Assignment 8: Complete Compiler Pipeline

## Overview
This assignment integrates all previous components into a complete compiler pipeline that can compile WLP4 source code to executable MIPS binary. It demonstrates the full compilation process from source code to machine code.

## Components
- **a8p1/**: Complete compiler implementation
- **wlp4gen.cc**: Enhanced code generator with full WLP4 support
- **compile**: Compilation script that chains all phases
- **schoolcompiler**: Simplified compilation command

## Compilation Pipeline
The complete pipeline consists of the following phases:

1. **Lexical Analysis** (`wlp4scan`): Tokenizes source code
2. **Syntax Analysis** (`wlp4parse`): Builds parse tree
3. **Code Generation** (`wlp4gen`): Generates MIPS assembly
4. **Assembly** (`cs241.linkasm`): Converts assembly to binary
5. **Linking** (`cs241.linker`): Links with runtime libraries
6. **Final Assembly** (`cs241.merl`): Produces executable MIPS binary

## Usage
```bash
# Complete compilation pipeline
wlp4c < foo.wlp4 > test.mips

# Or step by step
cat foo.wlp4 | wlp4scan | wlp4parse | ./wlp4gen > foo.asm
cs241.linkasm < foo.asm > output.merl
cs241.linker output.merl print.merl alloc.merl > linked.merl
cs241.merl 0 < linked.merl > foo.mips
```

## Example
Complete WLP4 program:
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

Compilation process:
1. **Scanning**: Produces token stream
2. **Parsing**: Builds parse tree
3. **Code Generation**: Creates MIPS assembly
4. **Assembly**: Converts to binary
5. **Linking**: Links with runtime
6. **Execution**: Runs on MIPS simulator

## Key Features
- **Full WLP4 Support**: Handles all language constructs
- **Error Handling**: Comprehensive error reporting
- **Optimization**: Basic code optimizations
- **Runtime Integration**: Links with standard libraries
- **Memory Management**: Proper stack and heap handling

## Pipeline Integration
This assignment demonstrates how all previous components work together:
- **Assignment 4**: Lexical analysis
- **Assignment 6**: Syntax analysis  
- **Assignment 7**: Code generation
- **Assignment 3**: Assembly to binary
- **Assignment 1 & 2**: MIPS runtime support

## Dependencies
- All previous assignments
- Complete understanding of compiler phases
- Integration testing skills

