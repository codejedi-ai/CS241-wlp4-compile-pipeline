# Stage 2: Syntax Analysis (Parser)

## Purpose
Performs syntactic analysis using LR(1) parsing to build an Abstract Syntax Tree (AST) from the token stream.

## Files
- `wlp4parse.cc` - Parser implementation
- `samples/` - Token files and AST outputs

## What it does
1. Reads token stream from lexical analysis
2. Uses LR(1) grammar to validate syntax
3. Builds Abstract Syntax Tree (AST)
4. Reports syntax errors

## Input/Output
- **Input**: Token stream from scanner
- **Output**: Abstract Syntax Tree (AST)

## Example
**Input** (tokens):
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

**Output** (AST):
```
start BOF procedures EOF
procedures main
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
dcl type ID
type INT
INT int
ID a
...
```

## Usage
```bash
g++ -std=c++14 -Wall -o wlp4parse wlp4parse.cc
./wlp4scan < input.wlp4 | ./wlp4parse > output.ast
```
