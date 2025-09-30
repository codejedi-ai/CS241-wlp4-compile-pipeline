# How to Run Compiled MIPS Assembly Files

## Overview

The WLP4 compiler generates MIPS assembly code that can be executed using the included MIPS emulator. This approach provides direct execution without requiring additional compilation steps.

## Prerequisites

The MIPS emulator is included in the project and requires no additional installation:
- Built-in C++ MIPS processor simulation
- Supports all MIPS instructions used by the WLP4 compiler
- Implements runtime library functions (print, println, etc.)

## Running MIPS Assembly Files

### Method 1: Complete Compilation Pipeline

```bash
# Compile WLP4 to MIPS and execute
echo "int wain(int a, int b) { println(42); return 0; }" | \
  ./01_scan/wlp4scan | ./02_parse/wlp4parse | ./03_codegen/wlp4gen | \
  ./04_mips_emulator/mips_emulator
```

### Method 2: Step-by-Step Execution

```bash
# 1. Compile WLP4 to MIPS assembly
echo "int wain(int a, int b) { println(a + b); return 0; }" | \
  ./01_scan/wlp4scan | ./02_parse/wlp4parse | ./03_codegen/wlp4gen > output.asm

# 2. Execute with MIPS emulator
./04_mips_emulator/mips_emulator output.asm
```

### Method 3: Using Makefile

```bash
# Compile and execute in one step
make compile FILE=hello_world.wlp4
./04_mips_emulator/mips_emulator @completed/03_mips_asm/hello_world.asm
```

## Example: Hello World

**WLP4 Source (`hello_world.wlp4`):**
```c
int wain(int a, int b) {
    println(72);   // 'H'
    println(101); // 'e'
    println(108); // 'l'
    println(108); // 'l'
    println(111); // 'o'
    println(32);  // ' ' (space)
    println(87);  // 'W'
    println(111); // 'o'
    println(114); // 'r'
    println(108); // 'l'
    println(100); // 'd'
    println(10);  // '\n' (newline)
    return 0;
}
```

**Execution:**
```bash
# Compile and run
cat hello_world.wlp4 | ./01_scan/wlp4scan | ./02_parse/wlp4parse | \
  ./03_codegen/wlp4gen | ./04_mips_emulator/mips_emulator
```

**Output:**
```
Hello World
```

## MIPS Emulator Features

### Supported Instructions
- **Arithmetic**: add, sub, mult, div, mfhi, mflo
- **Memory**: lw, sw, lis
- **Branch**: beq, bne
- **Jump**: j, jal, jr, jalr
- **Comparison**: slt, slti
- **Pseudo-instructions**: move, nop

### Runtime Functions
- **print($reg)** - Print integer value
- **println($reg)** - Print integer with newline
- **new($reg)** - Allocate memory
- **delete($reg)** - Free memory
- **init** - Initialize memory allocator

### System Calls
- **syscall** - Based on $v0 value:
  - 1: Print integer ($a0)
  - 4: Print string ($a0)
  - 5: Read integer (result in $v0)
  - 10: Exit program

## File Locations

- **MIPS Emulator**: `04_mips_emulator/mips_emulator`
- **Generated Assembly**: `@completed/03_mips_asm/*.asm`
- **WLP4 Source Files**: `*.wlp4`

## Quick Test

```bash
# Test with a simple program
echo "int wain(int a, int b) { println(42); return 0; }" > test.wlp4
cat test.wlp4 | ./01_scan/wlp4scan | ./02_parse/wlp4parse | \
  ./03_codegen/wlp4gen | ./04_mips_emulator/mips_emulator
# Output: 42
```

## Advantages of MIPS Emulator Approach

1. **No Additional Compilation** - Direct execution of MIPS assembly
2. **Cross-Platform** - Works on any system with C++ compiler
3. **Educational Value** - Shows actual MIPS instruction execution
4. **Debugging** - Can inspect registers and memory during execution
5. **Runtime Support** - Built-in implementation of WLP4 runtime functions

## Troubleshooting

### Common Issues

1. **Permission Denied**: Make sure the emulator is executable
   ```bash
   chmod +x 04_mips_emulator/mips_emulator
   ```

2. **File Not Found**: Ensure all compiler stages are built
   ```bash
   make compiler-stages
   ```

3. **Parse Errors**: Check WLP4 syntax with individual stages
   ```bash
   cat your_file.wlp4 | ./01_scan/wlp4scan
   ```

### Debug Mode

The MIPS emulator can be modified to include debug output showing register states and instruction execution for educational purposes.

This approach provides a complete, self-contained solution for running WLP4 programs without external dependencies!