# WLP4 Scanner (Stage 1)

## Overview
The WLP4 Scanner is the first stage of the WLP4 compiler pipeline. It takes WLP4 source code as input and produces a stream of tokens that represent the lexical structure of the program.

## What it does
- **Lexical Analysis**: Breaks down WLP4 source code into tokens
- **Token Recognition**: Identifies keywords, identifiers, operators, literals, and punctuation
- **Error Detection**: Reports lexical errors (invalid characters, malformed tokens)
- **Whitespace Handling**: Filters out whitespace and comments

## Input Format
- **File Extension**: `.wlp4`
- **Content**: WLP4 source code (C-like syntax with specific WLP4 features)

### Example Input (`gcd.wlp4`):
```c
int wain(int a, int b) {
    int x = 0;
    int y = 0;
    int rem = 0;
    
    x = a;
    y = b;
    
    while (y != 0) {
        rem = x % y;
        x = y;
        y = rem;
    }
    
    return x;
}
```

## Output Format
- **File Extension**: `.scanned`
- **Format**: One token per line in the format `TOKEN_TYPE lexeme`
- **Token Types**: ID, NUM, LPAREN, RPAREN, LBRACE, RBRACE, RETURN, IF, ELSE, WHILE, PRINTLN, WAIN, BECOMES, INT, EQ, NE, LT, GT, LE, GE, PLUS, MINUS, STAR, SLASH, PCT, COMMA, SEMI, NEW, DELETE, LBRACK, RBRACK, AMP, NULL

### Example Output (`gcd.wlp4_scanned`):
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

## Usage

### Command Line
```bash
# Compile the scanner
make

# Run the scanner
./wlp4scan < input.wlp4 > output.scanned

# Or pipe input
echo "int wain() { return 0; }" | ./wlp4scan
```

### Program Structure
- **Main File**: `wlp4scan.cc`
- **Executable**: `wlp4scan`
- **Build System**: Makefile with C++14 support

## Key Features
- **DFA-based Tokenization**: Uses a deterministic finite automaton for efficient token recognition
- **Maximal Munch Algorithm**: Implements simplified maximal munch for token boundary detection
- **Keyword Recognition**: Automatically converts identifiers to keywords when appropriate
- **Error Handling**: Throws `ScanningFailure` exceptions for invalid input

## Token Types Supported
- **Keywords**: `int`, `wain`, `return`, `if`, `else`, `while`, `println`, `new`, `delete`, `NULL`
- **Operators**: `+`, `-`, `*`, `/`, `%`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&`
- **Punctuation**: `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`
- **Literals**: Integer numbers (`NUM`), identifiers (`ID`)
- **Special**: Comments (`//`), whitespace (filtered out)

## Error Handling
The scanner will report errors for:
- Invalid characters not recognized by the DFA
- Malformed tokens that cannot be parsed
- Unexpected end of input

## Dependencies
- **Compiler**: g++ with C++14 support
- **Standard Library**: iostream, string, vector, sstream, iomanip, cctype, algorithm, utility, set, array

## Build Instructions
```bash
# Clean build
make clean
make

# The executable will be created as 'wlp4scan'
```

## Integration
This scanner is designed to work as the first stage in a multi-stage compiler pipeline:
1. **Scanner** (this stage) → Token stream
2. **Parser** → Parse tree
3. **Code Generator** → Assembly code

The output of this stage feeds directly into the parser stage.
