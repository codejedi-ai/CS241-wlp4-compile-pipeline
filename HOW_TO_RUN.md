# How to Run Compiled x86 Assembly Files

## Prerequisites

Make sure you have the required tools installed:
```bash
sudo apt update
sudo apt install -y nasm gcc-multilib
```

## Current Status

✅ **Working Example**: `simple_working.asm` - Demonstrates proper x86 assembly syntax
❌ **Generated Files**: The MIPS to x86 converter needs improvement for proper NASM syntax

## Running x86 Assembly Files

### Method 1: Manual Assembly and Linking

```bash
# 1. Assemble the .asm file to object file
nasm -f elf32 your_file.asm -o your_file.o

# 2. Link with GCC to create executable
gcc -m32 your_file.o -o your_file

# 3. Run the executable
./your_file
```

### Method 2: Using the Run Script

```bash
# Use the provided script (works for properly formatted files)
./run_x86.sh your_file.asm
```

## Example: Working Hello World

```bash
# Assemble and run the working example
nasm -f elf32 simple_working.asm -o executables/simple_working.o
gcc -m32 executables/simple_working.o -o executables/simple_working
./executables/simple_working
```

**Output:**
```
Hello World!
42
```

## Issues with Generated Files

The current MIPS to x86 converter generates files with syntax errors:

1. **Wrong Section Placement**: `.word` directives in wrong sections
2. **Invalid Syntax**: Malformed register names and memory addressing
3. **Missing Labels**: Incomplete label definitions

## Next Steps

To make the generated files runnable, the MIPS to x86 converter needs:

1. **Proper Section Management**: Move data to `.data` section
2. **Correct NASM Syntax**: Fix register names and addressing modes
3. **Label Handling**: Properly define and reference labels
4. **Function Calls**: Correct calling conventions

## File Locations

- **Working Example**: `simple_working.asm`
- **Generated Files**: `@completed/*.x86` (need syntax fixes)
- **Executables**: `executables/` directory
- **Run Script**: `run_x86.sh`

## Quick Test

```bash
# Test the working example
cd /home/darcy/CS241-wlp4-compile-pipeline
nasm -f elf32 simple_working.asm -o executables/test.o
gcc -m32 executables/test.o -o executables/test
./executables/test
```

This demonstrates that the x86 assembly approach works - we just need to improve the MIPS to x86 converter to generate proper NASM syntax!
