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
