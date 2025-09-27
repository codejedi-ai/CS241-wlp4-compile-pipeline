# Assignment 2: MIPS Assembly Functions and Procedures

## Overview
This assignment focuses on advanced MIPS assembly programming, particularly function calls, parameter passing, and stack management. Students learn to implement proper calling conventions and manage the runtime stack.

## Components
- **a2p1.asm**: Function definitions and parameter passing
- **a2p2.asm**: Stack frame setup and teardown
- **a2p3.asm**: Recursive function implementation
- **a2p4.asm**: Local variable management
- **a2p5.asm**: Nested function calls
- **a2p6.asm**: Array parameter passing
- **a2p7.asm**: Return value handling
- **a2p8.asm**: Complex control flow with functions
- **print.asm**: Utility function for output

## Key Learning Objectives
- MIPS calling conventions
- Stack frame management
- Parameter passing (registers vs stack)
- Return value handling
- Local variable storage
- Recursive function implementation
- Memory management for function calls

## Pipeline Position
Builds upon Assignment 1 by adding function call mechanisms that the compiler will need to generate.

## Usage
Assembly files can be assembled and linked together:
```bash
cs241.linkasm < a2p1.asm > output.merl
cs241.linker output.merl print.merl alloc.merl > linked.merl
cs241.merl 0 < linked.merl > final.mips
```

## Example
```mips
; Function to find maximum of two values
Max: ; parameters $1 $2, output on $3
    sw $1, -4($30)    ; save parameter
    sw $2, -8($30)    ; save parameter
    sw $31, -16($30)  ; save return address
    lis $31
    .word 20
    sub $30, $30, $31 ; allocate stack frame
    
    add $3, $1, $0    ; assume $1 is larger
    slt $4, $1, $2    ; check if $1 < $2
    beq $4, $0, 1     ; if $1 >= $2, skip
    add $3, $2, $0    ; else $2 is larger
    
    lis $31
    .word 20
    add $30, $30, $31 ; deallocate stack frame
    lw $31, -16($30)  ; restore return address
    jr $31            ; return
```

## Dependencies
- Assignment 1 (basic MIPS knowledge)
- Understanding of stack-based memory management

