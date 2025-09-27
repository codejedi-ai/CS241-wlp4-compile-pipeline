# Assignment 7: Code Generation (wlp4gen)

## Overview
This assignment implements the code generation phase of the compiler, which translates the parse tree produced by the parser into MIPS assembly code. This is where the high-level WLP4 language constructs are converted into low-level assembly instructions.

## Components
- **a7p1/**: Code generator implementation
- **wlp4gen.cc**: Main code generation logic
- **Makefile**: Build configuration

## Key Features
- **Parse Tree Traversal**: Walks the AST to generate code
- **MIPS Code Generation**: Produces MIPS assembly instructions
- **Register Management**: Allocates and manages MIPS registers
- **Memory Management**: Handles stack frames and local variables
- **Control Flow**: Generates branches, loops, and function calls
- **Expression Evaluation**: Converts expressions to assembly

## Code Generation Strategy
- **Recursive Descent**: Traverses parse tree recursively
- **Register Allocation**: Uses MIPS registers efficiently
- **Stack Management**: Implements proper stack frame handling
- **Label Generation**: Creates unique labels for jumps and functions
- **Constant Folding**: Optimizes constant expressions

## Supported WLP4 Constructs
- **Variables**: Local and parameter declarations
- **Expressions**: Arithmetic, logical, and comparison operations
- **Statements**: Assignment, conditional, loop, and return
- **Functions**: Function definitions and calls
- **Pointers**: Pointer arithmetic and dereferencing
- **Memory**: Dynamic allocation with new/delete

## Pipeline Position
Third phase of compilation - converts parse tree to assembly code.

## Usage
```bash
make                    # Compile the code generator
./wlp4gen < parse_tree  # Generate MIPS assembly
```

## Example
WLP4 input:
```c
int wain(int a, int b) {
    int sum = a + b;
    return sum;
}
```

Generated MIPS assembly:
```mips
; Function prologue
sw $1, -4($30)    ; save parameter a
sw $2, -8($30)    ; save parameter b
sw $31, -12($30)  ; save return address
lis $31
.word 12
sub $30, $30, $31 ; allocate stack frame

; int sum = a + b;
add $3, $1, $2    ; sum = a + b
sw $3, -4($30)    ; store sum on stack

; return sum;
lw $3, -4($30)    ; load sum into $3

; Function epilogue
lis $31
.word 12
add $30, $30, $31 ; deallocate stack frame
lw $31, -12($30)  ; restore return address
jr $31            ; return
```

## Key Implementation Details
- **AST Traversal**: Visits each node in the parse tree
- **Code Templates**: Uses predefined templates for common constructs
- **Register Spilling**: Handles cases where more registers are needed
- **Label Management**: Generates unique labels for control flow
- **Stack Operations**: Manages local variables on the stack

## Dependencies
- Assignment 1 & 2 (MIPS assembly knowledge)
- Assignment 6 (parse tree structure)
- Understanding of code generation techniques

