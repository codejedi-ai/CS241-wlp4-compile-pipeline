# MIPS Assembly Language Assembler

A MIPS assembler that converts assembly language source code into machine code, supporting both standalone binary files and MERL (Module Entry Record Language) files for linking.

## Features

- **MIPS Instruction Support**: Supports R-type, I-type, and J-type MIPS instructions
- **Label Support**: Handles local and external labels with proper symbol resolution
- **MERL File Generation**: Creates MERL files for modules with imports/exports
- **Command Line Interface**: Flexible output filename specification
- **Error Handling**: Comprehensive error reporting with detailed messages

## Supported Instructions

### R-Type Instructions
- `add`, `sub`, `slt`, `sltu` - Arithmetic and comparison operations
- `mult`, `multu`, `div`, `divu` - Multiplication and division
- `mfhi`, `mflo` - Move from HI/LO registers
- `jr`, `jalr` - Jump and link register operations

### I-Type Instructions
- `lw`, `sw` - Load and store word operations
- `beq`, `bne` - Branch on equal/not equal
- `lis` - Load immediate and skip

### Pseudo-Instructions
- `.word` - Data word directive
- `.import` - Import external symbol
- `.export` - Export symbol for external use

## Usage

### Basic Usage
```bash
# Assemble to default output file
./binasm < input.asm

# Specify output filename
./binasm outputfile < input.asm
```

### File Types

The assembler automatically determines the output format:

- **Binary Files** (`.bin`): For standalone programs without imports/exports
- **MERL Files** (`.merl`): For modules with `.import` or `.export` directives

### Examples

#### Simple Binary Assembly
```bash
echo "lis \$3
.word 0x1234
add \$1, \$2, \$3" | ./binasm myprogram
# Creates: myprogram (binary file)
```

#### MERL Module Assembly
```bash
echo ".import foo
.export bar
bar:
lis \$3
.word foo
add \$1, \$2, \$3" | ./binasm mymodule
# Creates: mymodule (MERL file)
```

## Input Format

### Assembly Syntax
- **Registers**: `$0` through `$31`
- **Labels**: Alphanumeric identifiers followed by `:`
- **Comments**: Lines starting with `;`
- **Directives**: `.word`, `.import`, `.export`

### Example Assembly Code
```assembly
; Simple program
start:
    lis $3
    .word 0x1000
    add $1, $2, $3
    beq $1, $0, start
```

### MERL Directives
```assembly
; Import external symbol
.import printf

; Export symbol for other modules
.export main

main:
    lis $3
    .word printf    ; Reference to imported symbol
    jr $31
```

## Output Format

### Binary Files
- Raw 32-bit big-endian machine code
- Direct execution format
- No linking information

### MERL Files
- MERL header (magic number, file size, code size)
- Machine code section
- Linker records (REL, ESR, ESD entries)
- Big-endian byte order throughout

## Building

```bash
make
```

This creates the `binasm` executable.

## Error Handling

The assembler provides detailed error messages for:
- Invalid instructions or syntax
- Undefined labels
- Duplicate labels
- Out-of-range immediate values
- Invalid register numbers

## Technical Details

- **Two-Pass Assembly**: First pass builds symbol table, second pass generates code
- **Big-Endian Output**: All multi-byte values written in big-endian format
- **PC-Relative Addressing**: Branch instructions use PC-relative addressing
- **Symbol Resolution**: Labels resolved to absolute addresses or marked for relocation

## File Structure

- `asm.cc` - Main assembler implementation
- `scanner.h` - Token definitions and scanner interface
- `scanner.cc` - Lexical analysis implementation
- `Makefile` - Build configuration

## License

This project is part of CS241 coursework and follows academic integrity guidelines.
