# WLP4 Parser (Stage 2)

## Overview
The WLP4 Parser is the second stage of the WLP4 compiler pipeline. It takes a stream of tokens from the scanner and produces a parse tree that represents the syntactic structure of the WLP4 program according to the WLP4 grammar.

## What it does
- **Syntax Analysis**: Validates that the token stream conforms to the WLP4 grammar
- **Parse Tree Construction**: Builds a hierarchical parse tree representing the program structure
- **Error Detection**: Reports syntax errors and parsing failures
- **Grammar Validation**: Ensures the input follows the WLP4 language specification

## Input Format
- **File Extension**: `.scanned` (output from scanner stage)
- **Content**: Token stream in the format `TOKEN_TYPE lexeme` (one per line)

### Example Input (`gcd.wlp4_scanned`):
```
INT int
WAIN wain
LPAREN (
INT int
ID a
COMMA ,
INT int
ID b
RPAREN )
LBRACE {
INT int
ID x
BECOMES =
NUM 0
SEMI ;
...
```

## Output Format
- **File Extension**: `.parsed`
- **Format**: Parse tree in textual representation, showing the derivation tree
- **Structure**: Each line represents a production rule with its children

### Example Output (`gcd.wlp4_parsed`):
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

## Usage

### Command Line
```bash
# Compile the parser
make

# Run the parser
./wlp4parse < input.scanned > output.parsed

# Or pipe input from scanner
./wlp4scan < input.wlp4 | ./wlp4parse > output.parsed
```

### Program Structure
- **Main File**: `wlp4parse.cc`
- **Executable**: `wlp4parse`
- **Grammar File**: `WLP4.lr1` (LR(1) grammar specification)
- **Build System**: Makefile with C++14 support

## Key Features
- **LR(1) Parser**: Implements a bottom-up LR(1) parsing algorithm
- **Parse Tree Construction**: Builds a complete parse tree during parsing
- **Error Recovery**: Provides detailed error messages with line numbers
- **Grammar-Driven**: Uses the WLP4.lr1 grammar file for parsing decisions

## Grammar Specification
The parser uses the `WLP4.lr1` file which contains:
- **Terminals**: Token types (ID, NUM, LPAREN, etc.)
- **Non-terminals**: Grammar symbols (start, procedures, main, etc.)
- **Productions**: Grammar rules defining the language structure
- **LR(1) DFA**: State machine for parsing decisions

## Parse Tree Structure
The parse tree represents the syntactic structure:
- **Root**: `start` symbol
- **Nodes**: Grammar symbols (terminals and non-terminals)
- **Leaves**: Token lexemes
- **Hierarchy**: Shows how the program is derived from the grammar

## Error Handling
The parser reports errors for:
- **Syntax Errors**: Invalid token sequences
- **Unexpected Tokens**: Tokens that don't fit the grammar at the current state
- **Premature EOF**: End of input when more tokens are expected
- **Parse Failures**: When the LR(1) automaton cannot find a valid transition

### Error Format
```
ERROR at <line_number>
```

## WLP4 Grammar Highlights
The parser recognizes WLP4 language constructs:
- **Procedures**: Function definitions with parameters
- **Main Function**: Special `wain` function (entry point)
- **Statements**: Assignments, conditionals, loops, function calls
- **Expressions**: Arithmetic, logical, and comparison operations
- **Declarations**: Variable declarations with initialization
- **Types**: `int` and `int*` (pointer types)

## Dependencies
- **Compiler**: g++ with C++14 support
- **Standard Library**: iostream, sstream, string, vector, stack, map, queue, fstream, list
- **Grammar File**: `WLP4.lr1` (must be present)

## Build Instructions
```bash
# Clean build
make clean
make

# The executable will be created as 'wlp4parse'
```

## Integration
This parser is designed to work in the compiler pipeline:
1. **Scanner** → Token stream
2. **Parser** (this stage) → Parse tree
3. **Code Generator** → Assembly code

The parser consumes the token stream from the scanner and produces a parse tree for the code generator.

## Testing
The parser includes test files:
- `test1.in`: Sample input
- `test1.out`: Expected output
- `test1expected.out`: Reference output for comparison

## Algorithm Details
- **Bottom-up Parsing**: Uses LR(1) algorithm
- **State Stack**: Maintains parsing state
- **Symbol Stack**: Tracks parsed symbols
- **Reduce Actions**: Applies grammar productions
- **Shift Actions**: Consumes input tokens
