# Stage 4: Code Generation

## Purpose
Translates the validated AST into x86 assembly code.

## Files
- `wlp4gen_x86.cc` - Code generator implementation
- `samples/` - AST files and generated assembly

## What it does
1. Traverses the AST
2. Generates x86 assembly instructions
3. Handles register allocation
4. Implements calling conventions
5. Generates function prologue/epilogue

## Input/Output
- **Input**: Abstract Syntax Tree (AST)
- **Output**: x86 assembly code

## Example
**Input** (AST):
```
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
expr expr PLUS term
expr term
term factor
factor ID
ID a
PLUS +
term factor
factor ID
ID b
```

**Output** (x86 assembly):
```assembly
// x86 Assembly generated from WLP4
.section .text
.global wain
wain:
    push %ebp
    mov %esp, %ebp
    sub $16, %esp
    mov 8(%ebp), %eax  # load first parameter (a)
    mov %eax, -4(%ebp)  # store first parameter
    mov 12(%ebp), %eax  # load second parameter (b)
    mov %eax, -8(%ebp)  # store second parameter
    mov -4(%ebp), %eax  # load first parameter
    add -8(%ebp), %eax  # add second parameter
    mov %ebp, %esp
    pop %ebp
    ret
```

## Usage
```bash
g++ -std=c++14 -Wall -o wlp4gen_x86 wlp4gen_x86.cc
./wlp4gen_x86 < input.ast > output.s
```
