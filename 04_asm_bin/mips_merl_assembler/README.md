# MIPS MERL Assembler

A modular, object-oriented MIPS assembler that generates MERL (MIPS Executable and Relocatable Linker) format files.

## Project Structure

```
mips_merl_assembler/
├── src/                    # Source files (.cc)
│   ├── main.cc            # Main entry point
│   ├── assembler.cc        # Main assembler logic
│   ├── scanner.cc          # Lexical analyzer
│   ├── symbol_table.cc     # Symbol table management
│   ├── merl_entries.cc     # MERL entry generation
│   ├── code_generator.cc   # Code generation using factory pattern
│   ├── mips_instructions.cc # MIPS instruction factory
│   ├── merl_file.cc        # MERL file storage and output
│   └── instructions.cc     # Legacy instruction encoding
├── include/                # Header files (.h)
│   ├── scanner.h           # Scanner interface
│   ├── assembler.h         # Assembler interface
│   ├── symbol_table.h      # Symbol table interface
│   ├── merl_entries.h      # MERL entries interface
│   ├── code_generator.h    # Code generator interface
│   ├── mips_instructions.h # MIPS instruction hierarchy
│   ├── merl_file.h         # MERL file interface
│   └── instructions.h      # Legacy instruction interface
├── build/                  # Build artifacts (auto-generated)
├── Makefile               # Build configuration
└── README.md              # This file
```

## Features

### Object-Oriented Design
- **Factory Pattern**: `InstructionFactory` creates MIPS instruction objects
- **Inheritance Hierarchy**: Base `MIPSInstruction` class with specific instruction types
- **Polymorphism**: Virtual methods for encoding and description

### MIPS Instruction Categories
Based on MIPS_INSTRUCTIONS.md:
- **I-Type**: Data Movement & Memory Access (`lw`, `sw`, `beq`, `bne`, `addi`, etc.)
- **R-Type**: Arithmetic & Logic (`add`, `sub`, `mult`, `div`, `mfhi`, `mflo`, etc.)
- **Control Flow**: Branch & Jump (`jr`, `jalr`)
- **Special**: Directives (`.word`, `lis`)

### MERL File Support
- **Storage**: MERL files stored as `std::vector<uint32_t>`
- **Output**: Support for stdout, files, and hex dumps
- **Relocation**: REL, ESR, ESD entry generation
- **Debug**: Information and hex dump capabilities

## Building

```bash
make clean    # Clean build artifacts
make          # Build the project
```

The final executable `binmerl` is installed to `/home/darcy/binmerl`.

## Usage

```bash
# Assemble a MIPS assembly file
./binmerl < input.asm > output.merl

# Example assembly file (m1.asm):
.import foo
.export bar

sw $31, -4($30)
lis $29
.word foo
jalr $29
lis $3
.word bar
lw $3, 0($3)
lw $31, -4($30)
jr $31

bar: .word 0
```

## Architecture

### Assembly Process
1. **First Pass**: Build symbol tables and generate MERL entries
2. **Second Pass**: Generate code and build MERL file

### Key Components
- **SymbolTableManager**: Handles import/export/local symbols
- **MerlEntryGenerator**: Creates REL/ESR/ESD entries
- **CodeGenerator**: Uses factory pattern for instruction encoding
- **MerlFile**: Stores and outputs MERL format files

### Factory Pattern Implementation
The `InstructionFactory::createInstruction()` method:
- Parses tokenized assembly lines
- Creates appropriate instruction objects
- Returns polymorphic pointers to base class
- Provides comprehensive error handling

## Testing

The project includes a test program `test_merl_storage.cc` that demonstrates:
- MERL file storage capabilities
- Hex dump output
- File information display
- Debug output features
