# Stage 3: Semantic Analysis

## Purpose
Performs semantic analysis including type checking, variable declaration validation, and scope resolution.

## Files
- `samples/` - AST files and semantic analysis outputs

## What it does
1. Analyzes the AST for semantic correctness
2. Performs type checking
3. Validates variable declarations and usage
4. Checks procedure signatures
5. Reports semantic errors

## Input/Output
- **Input**: Abstract Syntax Tree (AST)
- **Output**: Validated AST with semantic information

## Key Checks
- Variable declaration before use
- Type compatibility
- Procedure signature matching
- Scope resolution
- No duplicate declarations

## Example
**Input** (AST):
```
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
dcl type ID
type INT
INT int
ID a
```

**Output**: Validated AST with type information and symbol table

## Note
This stage is typically integrated into the parser or code generator in our implementation.
