# WLP4 Code Generator (Stage 3)

## Overview
The WLP4 Code Generator is the final stage of the WLP4 compiler pipeline. It takes a parse tree from the parser and generates MIPS assembly code that can be executed on a MIPS processor or simulator.

## What it does
- **Code Generation**: Converts parse tree to MIPS assembly instructions
- **Type Checking**: Performs semantic analysis and type validation
- **Symbol Table Management**: Tracks variables, functions, and their types
- **Memory Management**: Handles stack allocation and variable storage
- **Control Flow**: Generates code for loops, conditionals, and function calls

## Input Format
- **File Extension**: `.parsed` (output from parser stage)
- **Content**: Parse tree in textual representation

### Example Input (`gcd.wlp4_parsed`):
```
start BOF procedures EOF
BOF BOF
procedures main
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
INT int
WAIN wain
LPAREN (
dcl type ID
type INT
INT int
ID a
COMMA ,
dcl type ID
type INT
INT int
ID b
RPAREN )
LBRACE {
dcls dcls dcl BECOMES NUM SEMI
...
```

## Output Format
- **File Extension**: `.asm`
- **Content**: MIPS assembly code ready for execution

### Example Output (`gcd.asm`):
```assembly
.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $11
.word 1
beq $0, $0, wain
wain: ; begin prologue
sub $29, $30, $4 ; setup frame pointer
sub $30 , $30, $4 ; update stack pointer
sw $1, 0($29) ; push variable a
sub $30 , $30, $4 ; update stack pointer
sw $2, -4($29) ; push variable b
; end prologue
sw $2, -4($30) ; push the value of $2 onto the stack
sub $30, $30, $4 ; -----------
add $2, $0, $0
sw $31, -4($30) ; push the value of $31 onto the stack
sub $30, $30, $4 ; -----------
lis $5
.word init
jalr $5
add $30, $30, $4  ; 
lw $31,-4($30) ; -----------
add $30, $30, $4  ; 
lw $2,-4($30) ; -----------
...
```

## Usage

### Command Line
```bash
# Compile the code generator
make

# Run the code generator
./wlp4gen < input.parsed > output.asm

# Complete pipeline
./wlp4scan < input.wlp4 | ./wlp4parse | ./wlp4gen > output.asm
```

### Program Structure
- **Main File**: `wlp4gen.cc`
- **Executable**: `wlp4gen`
- **Build System**: Makefile with C++11 support

## Key Features
- **Semantic Analysis**: Type checking and validation
- **Symbol Table Construction**: Tracks variables and functions
- **MIPS Code Generation**: Produces executable assembly code
- **Stack Management**: Handles function calls and local variables
- **Memory Layout**: Manages variable storage and offsets

## Type System
The code generator enforces WLP4's type system:
- **int**: 32-bit signed integers
- **int***: Pointers to integers
- **Type Checking**: Ensures type safety in expressions and assignments
- **Function Signatures**: Validates parameter and return types

## Generated Code Features
- **Prologue/Epilogue**: Function entry and exit code
- **Stack Management**: Automatic stack pointer management
- **Register Usage**: Efficient use of MIPS registers ($1-$31)
- **Control Flow**: Branches and jumps for loops and conditionals
- **Function Calls**: Support for procedure calls with parameters

## MIPS Assembly Output
The generated code includes:
- **Import Statements**: External function imports (print, init, new, delete)
- **Initialization**: Register setup and constants
- **Function Definitions**: Complete function implementations
- **Stack Operations**: Push/pop operations for temporary values
- **Arithmetic**: MIPS instructions for math operations
- **Memory Access**: Load/store operations for variables

## Error Handling
The code generator reports errors for:
- **Type Mismatches**: Incompatible types in expressions
- **Undeclared Variables**: Use of undefined identifiers
- **Duplicate Declarations**: Multiple definitions of the same variable
- **Invalid Function Calls**: Wrong number or types of parameters
- **Return Type Errors**: Invalid return values

### Error Format
```
ERROR: <error_description>
```

## Symbol Table Management
- **Function Tables**: Tracks function signatures and local variables
- **Variable Scoping**: Handles local and parameter variables
- **Offset Calculation**: Computes stack offsets for variables
- **Type Information**: Maintains type information for all symbols

## Memory Layout
- **Stack Frame**: Each function has its own stack frame
- **Variable Storage**: Local variables stored at negative offsets from frame pointer
- **Parameter Passing**: Parameters passed via registers and stack
- **Return Values**: Function results returned in register $3

## Dependencies
- **Compiler**: g++ with C++11 support
- **Standard Library**: iostream, sstream, string, vector, stack, map, queue

## Build Instructions
```bash
# Clean build
make clean
make

# The executable will be created as 'wlp4gen'
```

## Integration
This code generator completes the compiler pipeline:
1. **Scanner** → Token stream
2. **Parser** → Parse tree
3. **Code Generator** (this stage) → Assembly code

The code generator consumes the parse tree and produces executable MIPS assembly.

## Generated Code Structure
- **Header**: Import statements and initialization
- **Main Function**: Entry point (`wain`)
- **Procedures**: User-defined functions
- **Runtime Support**: Calls to external functions (print, init, new, delete)

## Testing
The code generator includes test files:
- `test1.mips`: Sample generated assembly
- `test1.parsed`: Sample parse tree input

## Algorithm Details
- **Tree Traversal**: Recursive traversal of parse tree
- **Code Generation**: Template-based code generation for each grammar rule
- **Register Allocation**: Simple register allocation strategy
- **Stack Management**: Automatic stack pointer updates
- **Label Generation**: Unique labels for control flow
