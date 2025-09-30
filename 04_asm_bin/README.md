# Enhanced Assembler Pipeline - Modular MIPS Toolchain

## Overview

This is a **modular MIPS assembly language toolchain** designed for CS241 coursework. It consists of three main components organized in separate directories:

- **`assembler/`** - Enhanced assembler with MERL output and import/export support
- **`linker/`** - Links multiple MERL files together  
- **`loader/`** - MERL file processor (converts MERL to binary)
- **`examples/`** - Sample assembly files and build scripts

## Directory Structure

```
enhanced_assembler/
├── assembler/           # Assembler component
│   ├── linkasm.cc      # Main assembler implementation
│   ├── scanner.cc       # Lexical analyzer
│   ├── scanner.h        # Token definitions
│   ├── Makefile         # Assembler build config
│   └── linkasm          # Built assembler executable
├── linker/              # Linker component
│   ├── linker.cc        # MERL file linker
│   ├── Makefile         # Linker build config
│   └── linker           # Built linker executable
├── loader/              # Loader component
│   ├── merl.cc          # MERL file processor
│   ├── Makefile         # Loader build config
│   └── merl             # Built loader executable
├── examples/            # Sample files and demos
│   ├── main.asm         # Main program with imports
│   ├── library.asm      # Library with exports
│   ├── sample.asm       # Basic assembly example
│   ├── build.sh         # Complete pipeline demo
│   └── *.merl, *.bin    # Generated files
└── Makefile             # Main build system
```

## Quick Start

### Build Everything
```bash
make
```

### Test Complete Pipeline
```bash
make test
```

### Run Demo
```bash
cd examples
./build.sh
```

## Linking Workflow

### When Do You Need Linking?

**YES, you need linking if your assembly files contain:**
- `.import symbol` - References external symbols
- `.export symbol` - Provides symbols for other modules

### Complete Linking Process

1. **Assemble** each module to MERL:
```bash
./assembler/linkasm < main.asm > main.merl
./assembler/linkasm < library.asm > library.merl
```

2. **Link** modules together:
```bash
./linker/linker main.merl library.merl > linked.merl
```

3. **Process** MERL to binary:
```bash
./loader/merl 0 < linked.merl > final.bin
```

## Individual Components

### Assembler (`assembler/`)
- **Purpose**: Converts assembly code to MERL format
- **Features**: Import/export support, MERL headers, symbol tables
- **Usage**: `./assembler/linkasm < input.asm > output.merl`

### Linker (`linker/`)
- **Purpose**: Combines multiple MERL files
- **Features**: File concatenation, basic linking
- **Usage**: `./linker/linker file1.merl file2.merl ... > output.merl`

### Loader (`loader/`)
- **Purpose**: Converts MERL files to executable binary
- **Features**: MERL parsing, code extraction
- **Usage**: `./loader/merl <address> < input.merl > output.bin`

#### Load Address Parameter

The `<address>` parameter specifies where the code should be loaded in memory:

```bash
./loader/merl 0 < input.merl > output.bin           # Load at address 0
./loader/merl 0x400000 < input.merl > output.bin     # Load at 4MB mark
./loader/merl 0x1000 < input.merl > output.bin       # Load at 4KB mark
```

**Important**: In the current implementation, the load address parameter is **required but ignored**. All addresses produce identical output. This is a simplified loader for educational purposes.

**Common Values**:
- `0` - Load at memory address 0 (kernel space)
- `0x400000` - Load at 4MB mark (user space programs)  
- `0x1000` - Load at 4KB mark (libraries)

**For CS241 coursework, use `0` as the load address.**

## Supported Assembly Features

### Instructions
- **Data**: `.word value`
- **Control**: `jr $31`
- **Comments**: `; comment text`

### Linking Directives
- **Import**: `.import symbol` - Use external symbol
- **Export**: `.export symbol` - Provide symbol to others

### Labels
- **Definition**: `label:` - Define a label

## Example: Modular Programming

### Main Program (`examples/main.asm`)
```assembly
; Main program that imports functions
.export main
.import helper
.import printf

main:
    .word 42
    .word 84
    jr $31
```

### Library Module (`examples/library.asm`)
```assembly
; Library that exports functions
.export helper
.export utility

helper:
    .word 100
    jr $31

utility:
    .word 200
    jr $31
```

### Build Process
```bash
# 1. Assemble each module
./assembler/linkasm < main.asm > main.merl
./assembler/linkasm < library.asm > library.merl

# 2. Link them together
./linker/linker main.merl library.merl > linked.merl

# 3. Convert to binary
./loader/merl 0 < linked.merl > final.bin
```

## Build Commands

### Main Build System
```bash
make              # Build all components
make clean        # Clean all components
make test         # Test complete pipeline
```

### Individual Components
```bash
make assembler    # Build just assembler
make linker       # Build just linker  
make loader       # Build just loader
```

### Component-Specific
```bash
cd assembler && make     # Build assembler only
cd linker && make        # Build linker only
cd loader && make        # Build loader only
```

## MERL File Format

MERL (MIPS Executable and Relocatable Linker) files contain:

- **Header**: Magic number (`0x10000002`) + code size
- **Code Section**: Actual assembly instructions/data
- **End Marker**: MERL end (`0x10000001`)

## Current Status

### ✅ Working Features
- **Modular Structure**: Clean separation of components
- **MERL Output**: Proper MERL format generation
- **Import/Export**: Symbol tracking and linking directives
- **Linking**: Multi-file MERL combination
- **Pipeline**: Complete assembler → linker → loader workflow
- **Build System**: Unified and component-specific builds

### ⚠️ Known Issues
- **Register Parsing**: Most arithmetic instructions fail (register parsing bug)
- **Symbol Resolution**: Import/export symbols tracked but not resolved
- **Simplified Linking**: No relocation or advanced symbol resolution

### 🔧 Limitations
- Limited MIPS instruction set
- No floating-point support
- No pseudo-instructions
- Basic linking (no relocation)

## Dependencies

- C++14 compatible compiler (GCC 4.9+ or Clang 3.4+)
- Standard C++ libraries
- Make build system

## License

This project is part of CS241 coursework and is intended for educational purposes.