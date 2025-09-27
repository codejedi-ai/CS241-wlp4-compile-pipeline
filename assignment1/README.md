# Assignment 1: MIPS Assembly Programming

## Overview
This assignment introduces MIPS assembly language programming fundamentals. Students learn to write basic MIPS assembly programs that perform arithmetic operations, memory management, and control flow.

## Components
- **a1p2.mips**: Basic arithmetic operations (addition, subtraction)
- **a1p3.mips**: Memory operations (load/store)
- **a1p4.mips**: Control flow (branches, jumps)
- **a1p5.mips**: Function calls and stack management
- **a1p6.mips**: Loops and conditional statements
- **a1p7.mips**: Array operations and pointer arithmetic
- **helloworld.mips**: Simple "Hello World" program

## Key Learning Objectives
- Understanding MIPS instruction set architecture
- Register usage conventions ($1-$31)
- Memory addressing modes
- Stack frame management
- Basic control flow structures

## Pipeline Position
This is the foundation layer - students learn the target assembly language that the compiler will generate.

## Usage
Each `.mips` file contains MIPS assembly code that can be assembled and executed using the CS241 MIPS simulator.

## Example
```mips
; Simple addition program
add $3, $1, $2    ; $3 = $1 + $2
jr $31            ; return
```

## Dependencies
- CS241 MIPS assembler and simulator
- Understanding of basic computer architecture concepts

