# Quick Start Guide

## Do I Need Linking?

**YES, if your assembly files contain:**
- `.import symbol` - You're using external symbols
- `.export symbol` - You're providing symbols to others

**NO, if your assembly files only contain:**
- Basic instructions (`jr $31`, `.word 42`)
- Local labels (`main:`, `loop:`)
- Comments (`; comment`)

## Quick Commands

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

### Step 1: Assemble Each Module
```bash
./assembler/linkasm < main.asm > main.merl
./assembler/linkasm < library.asm > library.merl
```

### Step 2: Link Modules
```bash
./linker/linker main.merl library.merl > linked.merl
```

### Step 3: Convert to Binary
```bash
./loader/merl 0 < linked.merl > final.bin
```

**Note**: The `0` is the load address parameter. Any value works (0, 1000, 0x400000) since it's currently ignored in this simplified implementation.

## Directory Structure

- **`assembler/`** - Assembler (assembly → MERL)
- **`linker/`** - Linker (MERL + MERL → MERL)  
- **`loader/`** - Loader (MERL → binary)
- **`examples/`** - Sample files and demos

## Example Files

- **`examples/main.asm`** - Program with imports
- **`examples/library.asm`** - Library with exports
- **`examples/build.sh`** - Complete demo script
