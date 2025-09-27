# Assignment 6: LR(1) Parser Implementation

## Overview
This assignment implements a bottom-up LR(1) parser that can handle more complex grammars than the top-down parser from Assignment 5. It uses lookahead symbols to make parsing decisions and can handle left-recursive grammars efficiently.

## Components
- **a6p1.cfg-r**: Grammar specification with precedence rules
- **a6p2.cfg-r**: Extended grammar with more complex constructs
- **a6p3/**: LR(1) parser implementation
- **a6p4/**: Enhanced parser with error recovery
- **a6p5/**: Complete WLP4 parser
- **WLP4.lr1**: LR(1) grammar specification for WLP4 language
- **p3test**: Test program for parser validation

## Key Features
- **LR(1) Parsing**: Uses one token of lookahead for parsing decisions
- **Action Table**: Pre-computed parsing action table
- **Goto Table**: State transition table for non-terminals
- **Error Recovery**: Handles syntax errors gracefully
- **WLP4 Grammar**: Full grammar support for WLP4 language features

## Grammar Features Supported
- **Expressions**: Arithmetic, logical, and comparison operations
- **Statements**: Assignment, conditional, loop, and return statements
- **Declarations**: Variable and function declarations
- **Types**: Integer types and pointers
- **Control Flow**: if-else, while loops, function calls
- **Memory Management**: new/delete operations

## Pipeline Position
Enhanced syntax analysis phase - more powerful than Assignment 5's parser.

## Usage
```bash
make                    # Compile the LR(1) parser
./wlp4parse < input.wlp4
```

## Example
WLP4 input:
```c
int wain(int a, int b) {
    int result = a + b;
    if (result > 0) {
        return result;
    } else {
        return 0;
    }
}
```

Parse tree structure:
```
procedure
├── INT
├── ID(wain)
├── LPAREN
├── params
│   ├── dcl
│   │   ├── INT
│   │   └── ID(a)
│   ├── COMMA
│   └── dcl
│       ├── INT
│       └── ID(b)
├── RPAREN
└── dcl
    └── statements
        ├── assignment
        ├── if
        └── return
```

## Key Implementation Details
- **LR(1) Table Construction**: Builds action and goto tables from grammar
- **Stack-based Parsing**: Uses parsing stack to track state
- **Lookahead Processing**: Considers next token for parsing decisions
- **Reduce Actions**: Handles grammar rule reductions
- **Shift Actions**: Handles token consumption

## Dependencies
- Assignment 4 (lexical analysis)
- Assignment 5 (basic parsing concepts)
- Understanding of LR parsing algorithms
- Knowledge of grammar analysis techniques

