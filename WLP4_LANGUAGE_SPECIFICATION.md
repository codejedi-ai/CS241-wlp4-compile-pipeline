# WLP4 Programming Language Specification

## Overview

WLP4 is a strict subset of C++ designed for educational purposes. A WLP4 source file contains a sequence of procedure definitions, ending with the main procedure `wain`.

## Lexical Syntax

### Tokens

A WLP4 program consists of tokens optionally separated by whitespace. Valid tokens include:

#### Identifiers and Numbers
- **ID**: Letter followed by zero or more letters/digits (not reserved words)
- **NUM**: Single digit or multi-digit number (first digit ≠ 0, max value: 2³¹-1)

#### Reserved Words
- `wain`, `int`, `if`, `else`, `while`, `println`, `putchar`, `getchar`, `return`, `NULL`, `new`, `delete`

#### Punctuation
- **LPAREN**: `(`
- **RPAREN**: `)`
- **LBRACE**: `{`
- **RBRACE**: `}`
- **LBRACK**: `[`
- **RBRACK**: `]`
- **BECOMES**: `=`
- **COMMA**: `,`
- **SEMI**: `;`
- **AMP**: `&`

#### Operators
- **EQ**: `==`
- **NE**: `!=`
- **LT**: `<`
- **GT**: `>`
- **LE**: `<=`
- **GE**: `>=`
- **PLUS**: `+`
- **MINUS**: `-`
- **STAR**: `*`
- **SLASH**: `/`
- **PCT**: `%`

### Whitespace
- **SPACE**: ASCII 32
- **TAB**: ASCII 9
- **NEWLINE**: ASCII 10
- **COMMENT**: `//` followed by characters up to next newline

### Tokenization Rules
1. Choose longest prefix that is a token or whitespace
2. Add token to sequence, discard prefix
3. Repeat until end of input or invalid prefix

## Context-Free Grammar

### Nonterminal Symbols
```
{procedures, procedure, main, params, paramlist, type, dcl, dcls, 
 statements, lvalue, expr, statement, test, term, factor, arglist}
```

### Production Rules

#### Program Structure
```
procedures → procedure procedures
procedures → main

procedure → INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE

main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
```

#### Parameters
```
params →
params → paramlist

paramlist → dcl
paramlist → dcl COMMA paramlist
```

#### Types and Declarations
```
type → INT
type → INT STAR

dcls →
dcls → dcls dcl BECOMES NUM SEMI
dcls → dcls dcl BECOMES NULL SEMI

dcl → type ID
```

#### Statements
```
statements →
statements → statements statement

statement → lvalue BECOMES expr SEMI
statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE
statement → PRINTLN LPAREN expr RPAREN SEMI
statement → PUTCHAR LPAREN expr RPAREN SEMI
statement → DELETE LBRACK RBRACK expr SEMI
```

#### Expressions
```
expr → term
expr → expr PLUS term
expr → expr MINUS term

term → factor
term → term STAR factor
term → term SLASH factor
term → term PCT factor

factor → ID
factor → NUM
factor → NULL
factor → LPAREN expr RPAREN
factor → AMP lvalue
factor → STAR factor
factor → NEW INT LBRACK expr RBRACK
factor → GETCHAR LPAREN RPAREN
factor → ID LPAREN RPAREN
factor → ID LPAREN arglist RPAREN

arglist → expr
arglist → expr COMMA arglist
```

#### Tests
```
test → expr EQ expr
test → expr NE expr
test → expr LT expr
test → expr LE expr
test → expr GE expr
test → expr GT expr
```

#### L-values
```
lvalue → ID
lvalue → STAR factor
lvalue → LPAREN lvalue RPAREN
```

## Context-Sensitive Syntax (Semantic Rules)

### Names & Identifiers

#### Procedure Declarations
- Two procedures with the same name cannot be declared
- A procedure cannot be called until it has been declared
- No mutual recursion allowed
- `wain` cannot call itself recursively

#### Variable Declarations
- Two IDs with the same name cannot be declared within the same procedure
- An ID cannot be used unless declared within the same procedure
- An ID may have the same name as a procedure (variable takes precedence)

#### Procedure Calls
- A procedure `x` cannot be called from within procedure `p` if there is an ID `x` declared in `p`

### Types

#### Type System
- **int**: Integer type
- **int***: Pointer to integer type

#### Type Rules

##### Basic Types
- `NUM` has type `int`
- `NULL` has type `int*`
- `ID` has the same type as its declaration

##### Operators
- `AMP lvalue`: `int*` (lvalue must be `int`)
- `STAR factor`: `int` (factor must be `int*`)
- `NEW INT LBRACK expr RBRACK`: `int*` (expr must be `int`)
- `GETCHAR LPAREN RPAREN`: `int`

##### Arithmetic
- `expr PLUS term`:
  - `int + int` → `int`
  - `int* + int` → `int*`
  - `int + int*` → `int*`
- `expr MINUS term`:
  - `int - int` → `int`
  - `int* - int` → `int*`
  - `int* - int*` → `int`

##### Procedure Calls
- `ID LPAREN RPAREN`: `int` (procedure must have empty signature)
- `ID LPAREN arglist RPAREN`: `int` (argument types must match signature)

#### Semantic Constraints
- Second parameter of `wain` must be `int`
- Return expression of `procedure` must be `int`
- Return expression of `main` must be `int`
- Assignment: `lvalue` and `expr` must have same type
- `PRINTLN`/`PUTCHAR`: argument must be `int`
- `DELETE`: argument must be `int*`
- Tests: both expressions must have same type
- Initialization: `NUM` → `int`, `NULL` → `int*`

## Behavior

### C++ Compatibility
WLP4 programs are valid C++ program fragments. Behavior depends on parameter types:

#### Integer Parameters (`wain(int, int)`)
```cpp
int wain(int, int);
void println(int);
int putchar(int);
int getchar(void);

// === Insert WLP4 Program Here ===

#include <stdlib.h>
#include <stdio.h>
int main(int argc, char** argv) {
  int a,b,c;
  printf("Enter first integer: ");
  scanf("%d", &a);
  printf("Enter second integer: ");
  scanf("%d", &b);
  c = wain(a,b);
  printf("wain returned %d\n", c);
  return 0;
}
void println(int x){
   printf("%d\n",x);
}
```

#### Pointer Parameters (`wain(int*, int)`)
```cpp
int wain(int*, int);
void println(int);
int putchar(int);
int getchar(void);

// === Insert WLP4 Program Here ===

#include <stdlib.h>
#include <stdio.h>
int main(int argc, char** argv) {
  int l, c;
  int* a;
  printf("Enter length of array: ");
  scanf("%d", &l);
  a = (int*) malloc(l*sizeof(int));
  for(int i = 0; i < l; i++) {
    printf("Enter value of array element %d: ", i);
    scanf("%d", a+i);
  }
  c = wain(a,l);
  printf("wain returned %d\n", c);
  return 0;
}
void println(int x){
   printf("%d\n",x);
}
```

### WLP4-Specific Behavior

#### Memory Management
- **Failed allocation**: Returns `NULL` (instead of throwing exception)
- **NULL dereference**: Must crash the program
- **Out-of-bounds pointer arithmetic**: Allowed (same as normal pointer arithmetic)
- **Out-of-bounds dereference**: Undefined behavior

#### Standard Library Functions
- `putchar` and `getchar`: Standard C library behavior
- `println`: Custom function for integer output

## Example Programs

### Simple Binary Search Simulation
```wlp4
int wain(int a, int b) {
    if (a == 7) {
        println(0);  // Found at index 0
    } else {
        if (b == 7) {
            println(1);  // Found at index 1
        } else {
            println(2);  // Found at index 2
        }
    }
    return 0;
}
```

### Hello World
```wlp4
int wain(int a, int b) {
    println(72);   // 'H'
    println(101);  // 'e'
    println(108);  // 'l'
    println(108);  // 'l'
    println(111);  // 'o'
    println(32);   // ' ' (space)
    println(87);   // 'W'
    println(111);  // 'o'
    println(114);  // 'r'
    println(108);  // 'l'
    println(100);  // 'd'
    println(10);   // '\n' (newline)
    return 0;
}
```

## Compilation Pipeline

1. **Scanner**: Tokenizes input into WLP4 tokens
2. **Parser**: Builds parse tree from tokens
3. **Code Generator**: Generates MIPS assembly
4. **MIPS Emulator**: Executes MIPS assembly directly

## Key Limitations

- No arrays (use pointers instead)
- No strings (use individual characters)
- No floating-point numbers
- No function pointers
- No structs or classes
- No exceptions
- Limited control flow (if/else, while only)
- No preprocessor directives
- No global variables (except parameters)
