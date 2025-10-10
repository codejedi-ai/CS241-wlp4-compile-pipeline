# WLP4 Compiler

A complete compiler implementation for the WLP4 programming language, written in C++.

## Project Structure

```
1-wlp4-compiler/
├── wlp4compile.cc          # Main compiler source file
├── wlp4compile.h           # Header file
├── wlp4compile.o            # Compiled object file
├── wlp4compiler             # Executable compiler
├── Makefile                 # Build configuration
├── grammar/                 # Grammar definitions
│   └── WLP4.lr1            # LR(1) grammar file
└── README.md               # This file
```

## Features

This compiler implements a complete compilation pipeline for WLP4:

### Stage 1: Lexical Analysis (Scanner)
- Tokenizes WLP4 source code
- Handles identifiers, keywords, numbers, operators, and punctuation
- Supports comments and whitespace
- Provides detailed error reporting with line and column information

### Stage 2: Syntax Analysis (Parser)
- Uses LR(1) parsing with grammar from `grammar/WLP4.lr1`
- Builds parse tree from tokenized input
- Handles WLP4 language constructs including:
  - Function definitions and calls
  - Variable declarations
  - Control flow (if/else, while loops)
  - Expressions and statements
  - Type checking

### Stage 3: Code Generation
- Generates MIPS assembly code
- Handles stack frame management
- Implements function calls and returns
- Supports dynamic memory allocation
- Generates optimized assembly for various operations

## Building the Compiler

To build the compiler:

```bash
make
```

This will create the `wlp4compiler` executable.

To clean build artifacts:

```bash
make clean
```

## Usage

### Basic Usage

```bash
./wlp4compiler < input.wlp4 > output.asm
```

The compiler reads WLP4 source code from stdin and outputs MIPS assembly code to stdout.

### Example WLP4 Program

```wlp4
int wain(int a, int b) {
    int x = 0;
    int y = a + b;
    if (y > 10) {
        println(y);
    } else {
        println(x);
    }
    return y;
}
```

### Compilation Process

1. **Input**: WLP4 source code via stdin
2. **Lexical Analysis**: Tokenizes the input
3. **Syntax Analysis**: Parses tokens into a parse tree
4. **Semantic Analysis**: Performs type checking and symbol table management
5. **Code Generation**: Produces MIPS assembly code
6. **Output**: Assembly code via stdout

## Error Handling

The compiler provides comprehensive error reporting:

- **Lexical Errors**: Invalid characters, unterminated strings
- **Syntax Errors**: Invalid grammar constructs, missing tokens
- **Semantic Errors**: Type mismatches, undefined variables, duplicate declarations

Error messages include:
- Stage where error occurred
- Line and column numbers
- Detailed description of the problem
- Context information

## Language Features

### Supported Types
- `int`: 32-bit signed integers
- `int*`: Pointers to integers

### Control Structures
- `if/else` statements
- `while` loops
- Function calls and returns

### Built-in Functions
- `println(int)`: Print integer followed by newline
- `putchar(int)`: Print single character
- `getchar()`: Read single character
- `new int[expr]`: Dynamic memory allocation
- `delete[] expr`: Memory deallocation

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Assignment: `=`
- Address-of: `&`
- Dereference: `*`

## Dependencies

- C++14 compatible compiler (g++)
- Standard C++ libraries
- Unix/Linux environment

## Architecture

The compiler follows a traditional multi-pass architecture:

1. **Scanner**: Converts source text to tokens
2. **Parser**: Builds abstract syntax tree from tokens
3. **Semantic Analyzer**: Performs type checking and builds symbol tables
4. **Code Generator**: Produces target assembly code

## Implementation Details

- Uses deterministic finite automaton (DFA) for lexical analysis
- Implements LR(1) parsing algorithm
- Maintains symbol tables for scope management
- Generates stack-based assembly code
- Handles register allocation and stack frame management

## Testing

Test the compiler with various WLP4 programs:

```bash
# Test with a simple program
echo "int wain(int a, int b) { return a + b; }" | ./wlp4compiler

# Test with file input
./wlp4compiler < test.wlp4 > test.asm
```

## Future Enhancements

Potential improvements:
- Optimization passes
- Additional language features
- Better error recovery
- Interactive debugging support
- Cross-platform compatibility

## License

This project is part of CS241 coursework and is for educational purposes.
