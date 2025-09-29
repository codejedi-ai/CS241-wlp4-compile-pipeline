# MIPS Emulator

A simple MIPS processor emulator that reads MIPS assembly files line by line and simulates processor execution.

## Features

- **Complete Register Set**: Simulates all 32 MIPS registers ($0-$31)
- **Memory Simulation**: Basic memory access for load/store operations
- **Instruction Support**: 
  - Arithmetic: add, addi, sub, mult, div, mfhi, mflo
  - Memory: lw, sw, lis
  - Control Flow: beq, bne, j, jal, jr, jalr
  - Comparison: slt, slti
  - Runtime Functions: print, println, new, delete, init, syscall
- **Label Support**: Handles labels and jumps
- **Runtime Functions**: Built-in support for WLP4 runtime functions

## Building

```bash
make
```

## Usage

```bash
./mips_emulator <mips_assembly_file>
```

## Example

```bash
./mips_emulator ../@completed/03_mips_asm/simple_while.asm
```

## Supported Instructions

### Arithmetic
- `add rd, rs, rt` - Add rs + rt, store in rd
- `addi rt, rs, immediate` - Add rs + immediate, store in rt
- `sub rd, rs, rt` - Subtract rs - rt, store in rd
- `mult rs, rt` - Multiply rs * rt, store in $hi:$lo
- `div rs, rt` - Divide rs / rt, store quotient in $lo, remainder in $hi
- `mfhi rd` - Move from $hi to rd
- `mflo rd` - Move from $lo to rd

### Memory
- `lw rt, offset(rs)` - Load word from memory[rs + offset] to rt
- `sw rt, offset(rs)` - Store word from rt to memory[rs + offset]
- `lis rd` - Load immediate (followed by .word instruction)

### Control Flow
- `beq rs, rt, label` - Branch if rs == rt
- `bne rs, rt, label` - Branch if rs != rt
- `j label` - Jump to label
- `jal label` - Jump and link (save return address in $ra)
- `jr rs` - Jump to address in rs
- `jalr rs` - Jump and link to address in rs

### Comparison
- `slt rd, rs, rt` - Set rd to 1 if rs < rt, else 0
- `slti rt, rs, immediate` - Set rt to 1 if rs < immediate, else 0

### Runtime Functions
- `print reg` - Print value in register
- `println reg` - Print value in register with newline
- `new reg` - Allocate memory, store address in reg
- `delete reg` - Free memory at address in reg
- `init` - Initialize memory allocator
- `syscall` - System call based on $v0 value

## Implementation Details

- **Simple Memory Model**: Uses a map to simulate memory
- **Register Simulation**: All 32 registers plus named aliases
- **Label Resolution**: Two-pass assembly (collect labels, then execute)
- **Runtime Integration**: Built-in support for WLP4 runtime functions

This emulator provides a simple way to execute MIPS assembly directly without additional compilation steps.


The ISA:

# MIPS Commands Reference

This document provides a comprehensive reference for all MIPS assembly instructions supported by the WLP4 compiler and MIPS emulator.

## Table of Contents
1. [Register Conventions](#register-conventions)
2. [Arithmetic Instructions](#arithmetic-instructions)
3. [Memory Instructions](#memory-instructions)
4. [Branch Instructions](#branch-instructions)
5. [Jump Instructions](#jump-instructions)
6. [Comparison Instructions](#comparison-instructions)
7. [Runtime Functions](#runtime-functions)
8. [System Calls](#system-calls)
9. [Pseudo-Instructions](#pseudo-instructions)
10. [Directives](#directives)

## Register Conventions

### General Purpose Registers
- `$0` - Always contains zero
- `$1` - Assembler temporary
- `$2-$3` - Function return values (`$v0`, `$v1`)
- `$4-$7` - Function arguments (`$a0`, `$a1`, `$a2`, `$a3`)
- `$8-$15` - Temporary registers (`$t0`-$t7`)
- `$16-$23` - Saved registers (`$s0`-$s7`)
- `$24-$25` - Temporary registers (`$t8`, `$t9`)
- `$26-$27` - Kernel registers (`$k0`, `$k1`)
- `$28` - Global pointer (`$gp`)
- `$29` - Stack pointer (`$sp`)
- `$30` - Frame pointer (`$fp`)
- `$31` - Return address (`$ra`)

### Special Registers
- `$hi` - High part of multiply/divide result
- `$lo` - Low part of multiply/divide result

## Arithmetic Instructions

### Addition
```assembly
add $rd, $rs, $rt     # $rd = $rs + $rt
addi $rt, $rs, imm    # $rt = $rs + immediate
```

**Examples:**
```assembly
add $1, $2, $3        # $1 = $2 + $3
addi $1, $2, 100      # $1 = $2 + 100
```

### Subtraction
```assembly
sub $rd, $rs, $rt     # $rd = $rs - $rt
```

**Example:**
```assembly
sub $1, $2, $3        # $1 = $2 - $3
```

### Multiplication
```assembly
mult $rs, $rt         # $hi:$lo = $rs * $rt
mfhi $rd              # $rd = $hi
mflo $rd              # $rd = $lo
```

**Example:**
```assembly
mult $2, $3           # Multiply $2 and $3
mflo $1               # $1 = low 32 bits of result
mfhi $4               # $4 = high 32 bits of result
```

### Division
```assembly
div $rs, $rt          # $lo = $rs / $rt, $hi = $rs % $rt
```

**Example:**
```assembly
div $2, $3             # Divide $2 by $3
mflo $1                # $1 = quotient
mfhi $4                # $4 = remainder
```

## Memory Instructions

### Load Word
```assembly
lw $rt, offset($rs)   # $rt = memory[$rs + offset]
```

**Example:**
```assembly
lw $1, 4($29)         # Load word from stack+4 into $1
```

### Store Word
```assembly
sw $rt, offset($rs)   # memory[$rs + offset] = $rt
```

**Example:**
```assembly
sw $1, -4($29)        # Store $1 to stack-4
```

### Load Immediate
```assembly
lis $rd               # Load immediate (requires .word directive)
.word value           # Immediate value
```

**Example:**
```assembly
lis $1                # Load immediate into $1
.word 42              # Value is 42
```

## Branch Instructions

### Branch on Equal
```assembly
beq $rs, $rt, label   # Branch if $rs == $rt
```

**Example:**
```assembly
beq $1, $2, loop      # Jump to 'loop' if $1 == $2
```

### Branch on Not Equal
```assembly
bne $rs, $rt, label   # Branch if $rs != $rt
```

**Example:**
```assembly
bne $1, $2, loop      # Jump to 'loop' if $1 != $2
```

## Jump Instructions

### Jump
```assembly
j label               # Jump to label
```

**Example:**
```assembly
j end                 # Jump to 'end' label
```

### Jump and Link
```assembly
jal label             # Jump to label, save return address in $ra
```

**Example:**
```assembly
jal function          # Call function, save return address
```

### Jump Register
```assembly
jr $rs                # Jump to address in $rs
```

**Example:**
```assembly
jr $ra                # Return from function
```

### Jump and Link Register
```assembly
jalr $rs              # Jump to address in $rs, save return address in $ra
```

**Example:**
```assembly
jalr $1               # Call function at address in $1
```

## Comparison Instructions

### Set Less Than
```assembly
slt $rd, $rs, $rt     # $rd = ($rs < $rt) ? 1 : 0
slti $rt, $rs, imm    # $rt = ($rs < immediate) ? 1 : 0
```

**Examples:**
```assembly
slt $1, $2, $3        # $1 = 1 if $2 < $3, else 0
slti $1, $2, 10       # $1 = 1 if $2 < 10, else 0
```

## Runtime Functions

### Print Functions
```assembly
print $rs             # Print integer in $rs
println $rs           # Print integer in $rs with newline
```

**Examples:**
```assembly
print $1              # Print value in $1
println $2            # Print value in $2 and newline
```

### Memory Management
```assembly
new $rd               # Allocate memory, address in $rd
delete $rs            # Free memory at address in $rs
init                  # Initialize memory allocator
```

**Examples:**
```assembly
new $1                # Allocate memory, address in $1
delete $2             # Free memory at address in $2
init                  # Initialize allocator
```

## System Calls

### Syscall
```assembly
syscall               # System call based on $v0 value
```

**Syscall Numbers:**
- `1` - Print integer (`$a0`)
- `2` - Print float (`$a0`)
- `3` - Print double (`$a0`)
- `4` - Print string (`$a0`)
- `5` - Read integer (result in `$v0`)
- `10` - Exit program

**Example:**
```assembly
lis $v0               # Load syscall number
.word 1                # Print integer
add $a0, $0, $1       # Set argument
syscall               # Execute syscall
```

## Pseudo-Instructions

### Move
```assembly
move $rd, $rs         # $rd = $rs (equivalent to add $rd, $rs, $0)
```

**Example:**
```assembly
move $1, $2           # Copy $2 to $1
```

### No Operation
```assembly
nop                   # No operation (does nothing)
```

## Directives

### Import Runtime Functions
```assembly
.import function_name # Import runtime function
```

**Available Functions:**
- `print` - Print integer
- `println` - Print integer with newline
- `init` - Initialize memory allocator
- `new` - Allocate memory
- `delete` - Free memory
- `putchar` - Print character
- `getchar` - Read character
- `exit` - Exit program

**Example:**
```assembly
.import print
.import println
.import init
```

### Word Directive
```assembly
.word value           # Define word with value
```

**Example:**
```assembly
.word 42              # Define word with value 42
```

## Common Patterns

### Function Prologue
```assembly
sw $ra, -4($29)       # Save return address
sub $29, $29, $4      # Allocate stack space
```

### Function Epilogue
```assembly
add $29, $29, $4      # Deallocate stack space
lw $ra, -4($29)       # Restore return address
jr $ra                # Return
```

### Loop Pattern
```assembly
loop:                 # Loop label
    # Loop body
    addi $1, $1, 1    # Increment counter
    slti $2, $1, 10   # Check condition
    bne $2, $0, loop  # Branch if not done
```

### Conditional Pattern
```assembly
    slt $1, $2, $3    # Set condition
    beq $1, $0, else  # Branch if false
    # True branch
    j end
else:
    # False branch
end:
```

## Usage in WLP4 Compiler

The WLP4 compiler generates MIPS assembly code that uses these instructions. The MIPS emulator can execute the generated assembly directly without requiring additional compilation.

### Example WLP4 to MIPS Translation

**WLP4 Code:**
```c
int wain(int a, int b) {
    println(a + b);
    return 0;
}
```

**Generated MIPS Assembly:**
```assembly
.import println
lis $4
.word 4
lis $11
.word 1
sw $31, -4($30)
sub $30, $30, $4
add $1, $1, $2
sw $1, -8($30)
sub $30, $30, $4
jalr $10
add $30, $30, $4
lw $31, -4($30)
jr $31
```

This reference covers all MIPS instructions supported by the WLP4 compiler pipeline and MIPS emulator.
