# Converting Linked MERL to Executable Binary

## The Process

### Command
```bash
./loader/merl <load_address> < linked_program.merl > executable.bin
```

### Example
```bash
./loader/merl 0 < linked_program.merl > executable.bin
```

## What the Loader Does

### 1. Validates MERL Format
- Checks for MERL magic number: `0x10000002`
- Ensures file is valid MERL format

### 2. Reads Code Size
- Extracts code size from bytes 4-7
- Determines how much code to extract

### 3. Extracts Code Section
- Skips MERL header (first 8 bytes)
- Outputs only the actual MIPS instructions
- Ignores MERL end markers

## File Comparison

### Before (linked_program.merl - 48 bytes)
```
10 00 00 02 00 00 00 2c  64 c8 03 e0 00 08 2c 90  |.......,d.....,.|
03 e0 00 08 2a 03 e0 00  08 10 00 00 01 10 00 00  |....*...........|
02 00 00 00 20 0a 14 1e  03 e0 00 08 10 00 00 01  |.... ...........|
```

**Structure:**
- `10 00 00 02` = MERL Magic
- `00 00 00 2c` = Code Size (44 bytes)
- `64 c8 03 e0...` = MIPS Code
- `10 00 00 01` = MERL End

### After (executable.bin - 40 bytes)
```
64 c8 03 e0 00 08 2c 90  03 e0 00 08 2a 03 e0 00  |d.....,.....*...|
08 10 00 00 01 10 00 00  02 00 00 00 20 0a 14 1e  |............ ...|
03 e0 00 08 10 00 00 01                           |........|
```

**Structure:**
- Pure MIPS instructions only
- No MERL headers or metadata
- Ready for MIPS execution

## Complete Pipeline

```bash
# 1. Assemble modules
./assembler/linkasm < math_lib.asm > math_lib.merl
./assembler/linkasm < main_program.asm > main_program.merl

# 2. Link modules
./linker/linker math_lib.merl main_program.merl > linked_program.merl

# 3. Convert to executable binary
./loader/merl 0 < linked_program.merl > executable.bin
```

## Key Points

1. **MERL files are NOT executable** - they're relocatable object files
2. **The loader extracts pure MIPS code** from MERL format
3. **The binary is still MIPS code** - needs MIPS emulator/hardware to run
4. **Load address parameter** (0 in example) sets memory address for code
5. **Size reduction** - removes MERL metadata, keeps only instructions

## What You Get

- **executable.bin**: Pure MIPS 32-bit binary
- **Ready for**: MIPS emulator, MIPS hardware, or further processing
- **Not ready for**: Direct execution on x86_64 systems
