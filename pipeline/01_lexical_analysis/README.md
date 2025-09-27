# Stage 1: Lexical Analysis (Scanner)

## Purpose
Converts WLP4 source code into a stream of tokens by performing lexical analysis.

## Files
- `wlp4scan.cc` - Scanner implementation
- `samples/` - WLP4 source code samples

## What it does
1. Reads WLP4 source code character by character
2. Identifies tokens (keywords, identifiers, operators, literals)
3. Handles whitespace and comments
4. Outputs token stream with types and lexemes

## Input/Output
- **Input**: WLP4 source code
- **Output**: Token stream

## Example
**Input**:
```wlp4
int wain(int a, int b) {
    return a + b;
}
```

**Output**:
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
RETURN return
ID a
PLUS +
ID b
SEMI ;
RBRACE }
EOF EOF
```

## Usage
```bash
g++ -std=c++14 -Wall -o wlp4scan wlp4scan.cc
./wlp4scan < input.wlp4 > output.tokens
```
