# Assignment 5: Syntax Analysis (Parser)

## Overview
This assignment implements a top-down parser that performs syntax analysis on the token stream produced by the lexical analyzer. It uses a grammar-based approach to build a parse tree representing the syntactic structure of the WLP4 program.

## Components
- **galaxy.cc**: Main parser implementation
- **traverse.cc**: Parse tree traversal utilities
- **sample.cfg**: Example grammar specification
- **a5p2.cfg** through **a5p7.cfg**: Various grammar test cases
- **Makefile**: Build configuration

## Key Features
- **Grammar Processing**: Reads context-free grammar specifications
- **Parse Tree Construction**: Builds hierarchical tree structure from token stream
- **Error Detection**: Identifies syntax errors and reports them
- **Tree Traversal**: Implements various tree walking algorithms
- **Derivation Generation**: Can produce leftmost derivations

## Grammar Format
The parser reads grammar specifications in the following format:
```
<num_terminals>
<terminal1>
<terminal2>
...
<num_nonterminals>
<nonterminal1>
<nonterminal2>
...
<start_symbol>
<num_rules>
<rule1>
<rule2>
...
```

## Pipeline Position
Second phase of compilation - converts token stream into parse tree.

## Usage
```bash
make                    # Compile the parser
./galaxy < grammar.cfg < input.tokens
```

## Example
Grammar rule:
```
expr -> expr - term
expr -> term
term -> id
term -> ( expr )
```

Parse tree for `id - ( id )`:
```
S
└── BOF
└── expr
    ├── expr
    │   └── term
    │       └── id
    ├── -
    └── term
        ├── (
        ├── expr
        │   └── term
        │       └── id
        └── )
└── EOF
```

## Key Implementation Details
- **Recursive Descent**: Implements top-down parsing algorithm
- **Lookahead**: Uses first/follow sets for predictive parsing
- **Tree Building**: Constructs parse tree nodes during parsing
- **Error Recovery**: Attempts to recover from syntax errors
- **Grammar Validation**: Ensures grammar is well-formed

## Dependencies
- Assignment 4 (lexical analysis)
- Understanding of context-free grammars
- Knowledge of parsing algorithms

