# Assignment 4: Lexical Analysis (Scanner/DFA)

## Overview
This assignment implements the lexical analysis phase of the compiler, which converts source code into a stream of tokens. It uses Deterministic Finite Automata (DFA) to recognize different token types in the WLP4 language.

## Components
- **dfa.cc**: Main DFA implementation for token recognition
- **dfaMaker.cpp**: Tool to generate DFA tables from specifications
- **wlp4.dfa**: DFA specification for WLP4 language
- **int.dfa**: DFA for integer literals
- **not23.dfa**: Example DFA for specific patterns
- **notdiv3.dfa**: Example DFA for divisibility testing
- **pets.dfa**: Example DFA for pet names

## Key Features
- **Token Recognition**: Identifies keywords, identifiers, operators, literals
- **DFA Engine**: Generic DFA implementation for pattern matching
- **WLP4 Language Support**: Handles C-like syntax with specific extensions
- **Error Handling**: Detects and reports lexical errors
- **Flexible Design**: Supports multiple DFA specifications

## Supported Token Types
- **Keywords**: `int`, `if`, `else`, `while`, `return`, `wain`, `new`, `delete`, `NULL`
- **Identifiers**: Variable and function names
- **Literals**: Integer constants
- **Operators**: `+`, `-`, `*`, `/`, `%`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`
- **Delimiters**: `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`
- **Special**: `&`, `*` (for pointers)

## Pipeline Position
First phase of compilation - converts source code text into structured tokens.

## Usage
```bash
make                    # Compile the scanner
./dfa < input.wlp4      # Tokenize WLP4 source code
```

## Example
Input WLP4 code:
```c
int wain(int a, int b) {
    return a + b;
}
```

Output tokens:
```
INT ID(wain) LPAREN INT ID(a) COMMA INT ID(b) RPAREN LBRACE
RETURN ID(a) PLUS ID(b) SEMI RBRACE
```

## Key Implementation Details
- **DFA Table**: Pre-computed state transition table for efficient tokenization
- **State Management**: Tracks current state and transitions based on input characters
- **Token Classification**: Categorizes recognized patterns into appropriate token types
- **Error Recovery**: Handles invalid characters and malformed tokens

## Dependencies
- Understanding of finite automata theory
- Knowledge of lexical analysis concepts
- C++ programming skills

