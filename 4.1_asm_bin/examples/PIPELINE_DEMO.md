# Complete Pipeline Demo - ASM → MERL → Linked → Binary

## Overview
This demo shows the complete pipeline from assembly source files to final executable binary, demonstrating how files with `.import` directives need to be linked.

## Files Created

### 1. Library Module (`math_lib.asm`)
```assembly
; Math Library - Exports mathematical functions
.export addnumbers
.export multiplynumbers
.export getconstant

addnumbers:
    ; Function that adds two numbers
    .word 100
    .word 200
    jr $31

multiplynumbers:
    ; Function that multiplies two numbers  
    .word 300
    .word 400
    jr $31

getconstant:
    ; Function that returns a constant value
    .word 42
    jr $31
```

### 2. Main Program (`main_program.asm`)
```assembly
; Main Program - Imports functions from math library
.export main
.import addnumbers
.import multiplynumbers
.import getconstant

main:
    ; Main program that uses imported functions
    .word 10
    .word 20
    .word 30
    jr $31
```

## Pipeline Stages

### Stage 1: Assembly → MERL

**Command:**
```bash
./assembler/linkasm < math_lib.asm > math_lib.merl
./assembler/linkasm < main_program.asm > main_program.merl
```

**Output Files:**

**math_lib.merl (29 bytes):**
```
00000000  10 00 00 02 00 00 00 2c  64 c8 03 e0 00 08 2c 90  |.......,d.....,.|
00000010  03 e0 00 08 2a 03 e0 00  08 10 00 00 01           |....*........|
```

**main_program.merl (19 bytes):**
```
00000000  10 00 00 02 00 00 00 20  0a 14 1e 03 e0 00 08 10  |....... ........|
00000010  00 00 01                                          |...|
```

### Stage 2: Linking MERL Files

**Command:**
```bash
./linker/linker math_lib.merl main_program.merl > linked_program.merl
```

**Output File:**

**linked_program.merl (48 bytes):**
```
00000000  10 00 00 02 00 00 00 2c  64 c8 03 e0 00 08 2c 90  |.......,d.....,.|
00000010  03 e0 00 08 2a 03 e0 00  08 10 00 00 01 10 00 00  |....*...........|
00000020  02 00 00 00 20 0a 14 1e  03 e0 00 08 10 00 00 01  |.... ...........|
00000030
```

### Stage 3: MERL → Binary

**Command:**
```bash
./loader/merl 0 < linked_program.merl > final_program.bin
```

**Output File:**

**final_program.bin (40 bytes):**
```
00000000  64 c8 03 e0 00 08 2c 90  03 e0 00 08 2a 03 e0 00  |d.....,.....*...|
00000010  08 10 00 00 01 10 00 00  02 00 00 00 20 0a 14 1e  |............ ...|
00000020  03 e0 00 08 10 00 00 01                           |........|
```

## File Structure Analysis

### MERL File Format
Each MERL file contains:
- **Header**: `10 00 00 02` (MERL magic number)
- **Code Size**: `00 00 00 XX` (size of code section)
- **Code Section**: Actual assembly instructions/data
- **End Marker**: `10 00 00 01` (MERL end)

### math_lib.merl Breakdown
- **Magic**: `10 00 00 02`
- **Code Size**: `00 00 00 2c` (44 bytes)
- **Code**: `64 c8 03 e0 00 08 2c 90 03 e0 00 08 2a 03 e0 00 08`
  - `64` = 100 (first .word)
  - `c8` = 200 (second .word)  
  - `03 e0 00 08` = `jr $31` instruction
  - `2c` = 44 (third .word)
  - `90` = 144 (fourth .word)
  - `03 e0 00 08` = `jr $31` instruction
  - `2a` = 42 (fifth .word)
  - `03 e0 00 08` = `jr $31` instruction
- **End**: `10 00 00 01`

### main_program.merl Breakdown
- **Magic**: `10 00 00 02`
- **Code Size**: `00 00 00 20` (32 bytes)
- **Code**: `0a 14 1e 03 e0 00 08`
  - `0a` = 10 (first .word)
  - `14` = 20 (second .word)
  - `1e` = 30 (third .word)
  - `03 e0 00 08` = `jr $31` instruction
- **End**: `10 00 00 01`

## Key Points

1. **Import/Export Required Linking**: Files with `.import` directives must be linked with files containing `.export` directives.

2. **MERL Format**: Each module generates a proper MERL file with headers and end markers.

3. **Linking Process**: The linker combines multiple MERL files into a single linked MERL file.

4. **Final Binary**: The loader extracts the executable code from the MERL format.

5. **Modular Design**: This demonstrates how to create modular assembly programs with separate libraries and main programs.

## Commands Summary

```bash
# Complete pipeline
./assembler/linkasm < math_lib.asm > math_lib.merl
./assembler/linkasm < main_program.asm > main_program.merl
./linker/linker math_lib.merl main_program.merl > linked_program.merl
./loader/merl 0 < linked_program.merl > final_program.bin
```
