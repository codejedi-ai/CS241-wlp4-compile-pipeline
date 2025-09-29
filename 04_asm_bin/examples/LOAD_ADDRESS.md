# Load Address Parameter Explained

## What is `<load_address>`?

The `<load_address>` parameter tells the loader **where in memory** the code should be loaded for execution.

## Current Implementation

### What It Does Now
```bash
./loader/merl 0 < linked_program.merl > binary.bin
./loader/merl 1000 < linked_program.merl > binary.bin  
./loader/merl 0x400000 < linked_program.merl > binary.bin
```

**Result**: All produce **identical output** - the load address is **ignored**.

### Why It's Required But Not Used
- The loader **requires** the parameter (checks `argc != 2`)
- But it **doesn't use** the `loadAddress` variable
- This is a **simplified implementation** for CS241 coursework

## What Load Address SHOULD Do

### Memory Layout
```
Memory Address:    0x00000000  [Kernel Space]
                   ...
                   0x40000000  [User Space Start]
                   ...
                   0x7FFFFFFF  [User Space End]
```

### Examples

#### Load Address 0
```bash
./loader/merl 0 < program.merl > binary.bin
```
- Code loads at memory address `0x00000000`
- Absolute addresses in code remain unchanged
- Used for kernel code or boot loaders

#### Load Address 0x400000
```bash
./loader/merl 0x400000 < program.merl > binary.bin
```
- Code loads at memory address `0x00400000` (4MB mark)
- Common user space program start address
- Absolute addresses would be adjusted by loader

#### Load Address 0x1000
```bash
./loader/merl 0x1000 < program.merl > binary.bin
```
- Code loads at memory address `0x00001000` (4KB mark)
- Used for small programs or libraries

## What a Full Loader Would Do

### 1. Address Relocation
- Scan code for absolute memory references
- Adjust addresses based on load address
- Update jump targets and data references

### 2. Memory Layout
- Reserve space for code at specified address
- Handle memory alignment requirements
- Set up proper memory permissions

### 3. Symbol Resolution
- Resolve imported symbols to actual addresses
- Link external references
- Create executable memory image

## Current Behavior

### What Happens Now
1. **Parameter Required**: Loader checks for exactly 2 arguments
2. **Parameter Ignored**: `loadAddress` variable is never used
3. **Simple Extraction**: Only extracts code from MERL format
4. **No Relocation**: No address adjustment performed

### Why This Works for CS241
- **Educational Purpose**: Focuses on MERL format understanding
- **Simplified**: Avoids complex relocation logic
- **Sufficient**: Works for basic MIPS assembly programs
- **Demonstration**: Shows pipeline without full implementation

## Usage Examples

### Basic Usage (All Equivalent)
```bash
./loader/merl 0 < program.merl > binary.bin
./loader/merl 1 < program.merl > binary.bin
./loader/merl 999 < program.merl > binary.bin
```

### Common Conventions
```bash
# User space program
./loader/merl 0x400000 < program.merl > binary.bin

# Kernel code
./loader/merl 0 < kernel.merl > kernel.bin

# Library code
./loader/merl 0x1000 < library.merl > library.bin
```

## Key Points

1. **Currently Ignored**: Load address parameter doesn't affect output
2. **Required Parameter**: Must provide some value (any number works)
3. **Future Enhancement**: Could be implemented for full relocation
4. **Educational Tool**: Simplified for learning MERL format
5. **MIPS Specific**: Designed for MIPS architecture memory layout

## Summary

The `<load_address>` parameter is a **placeholder for future functionality**. In a complete implementation, it would control where the code is loaded in memory and perform address relocation. For now, it's required but ignored, making it safe to use any value like `0`.
