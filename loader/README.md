# MERL Loader

A MERL (MIPS Executable and Relocatable Linker) processor that serves as the final stage in the WLP4 compiler pipeline. The loader converts MERL object files into executable MIPS binary code by processing relocation records and resolving addresses.

## Overview

The loader is responsible for:
- **MERL File Processing**: Reading and parsing MERL object files
- **Address Resolution**: Fixing placeholder addresses in compiled code
- **Relocation Processing**: Applying relocation records to update memory references
- **Memory Management**: Simulating RAM allocation and load address calculation
- **Format Conversion**: Converting MERL object files to executable MIPS binary

## Architecture

The loader consists of two main components:

### 1. `main.cc` - Main Orchestrator
- **Input**: MERL file path and load address as command-line arguments
- **Process**: Reads MERL file, calls merl loader, outputs binary
- **Output**: Raw MIPS binary code to stdout

### 2. `merl.cc` - MERL Processor Library
- **Function**: `load_merl(data, loadAddress)` - processes MERL data and returns uint32 array
- **Input**: Vector of char data (MERL format) and load address
- **Output**: Vector of uint32_t (executable MIPS instructions)

## Usage

### Building
```bash
# Build the main executable
make

# Clean build files
make clean
```

### Command Line Usage
```bash
# Convert MERL file to executable binary
./main <merl_file> <load_address> > output.bin

# Example with hex address
./main program.merl 0x10000000 > program.bin

# Example with decimal address
./main program.merl 268435456 > program.bin
```

### Integration with Compiler Pipeline
```bash
# Complete compilation and loading
echo "int wain() { return 42; }" | \
  ./wlp4scan | ./wlp4parse | ./wlp4gen | \
  ./assembler program.merl | \
  ./main program.merl 0x10000000 > final.bin
```

## MERL Format

MERL is a binary format for MIPS object files containing:

### File Structure
```
[Magic Number][End Mod/Code Size][Code Data][Relocation Records][End Marker]
```

### Components
- **Magic Number**: `0x10000002` - Identifies MERL files
- **End Mod/Code Size**: File size information or code section size
- **Code Data**: Compiled MIPS assembly instructions (machine code)
- **Relocation Records**: Addresses that need to be fixed at load time
- **End Marker**: `0x10000001` - Marks end of file

## How It Works

### 1. MERL File Reading
```cpp
// Read MERL file as binary data
vector<char> merlData = read_merl_file(merlFile);
```

### 2. MERL Processing
```cpp
// Load MERL data using the merl function
vector<uint32_t> memory = load_merl(merlData, loadAddress);
```

### 3. Relocation Processing
```cpp
// Apply relocation: MEM[α + rel - 12] += (α - 12)
memory[relIndex] += alpha - 12;
```

### 4. Binary Output
```cpp
// Output the final binary
for (uint32_t word : memory) {
    cout << (char)(word >> 24);
    cout << (char)(word >> 16);
    cout << (char)(word >> 8);
    cout << (char)(word);
}
```

## Example

### Input MERL File (`program.merl`)
```
Magic:     0x10000002
EndMod:    0x00000020
CodeSize:  0x0000000C
Code:      [MIPS machine code]
Reloc:     Format=1, Address=0x00000008
EndMarker: 0x10000001
```

### Processing
```bash
./main program.merl 0x10000000 > program.bin
```

### Output Binary
```
00000000  00 00 00 11 ff ff ff f4
```

## Technical Details

### Dependencies
- **C++14**: Modern C++ features for robust implementation
- **Standard Library**: Vector, iostream, fstream, cstdint
- **Make**: Build system integration

### Data Types
- **Input**: MERL file (binary format)
- **Intermediate**: MERL data (vector<char>)
- **Output**: MIPS binary (vector<uint32_t>)

### Memory Management
- **Load Address**: Specified as command-line argument
- **Word Alignment**: All instructions are 4-byte aligned
- **Memory Simulation**: Uses vector<uint32_t> for RAM simulation

## Error Handling

The loader provides comprehensive error checking:

### File Validation
- MERL file existence and readability
- MERL magic number verification
- File size validation

### Processing Errors
- Invalid MERL format detection
- Memory bounds checking
- Relocation address validation

### Debug Output
- Format detection messages
- Relocation application logging
- Memory allocation information

## Integration with WLP4 Compiler

The loader is the final stage in the WLP4 compilation pipeline:

```
WLP4 Source → Scanner → Parser → Code Generator → Assembler → MERL File → Loader → Executable Binary
```

### Role in Compilation
1. **Assembler**: Produces MERL object files with placeholder addresses
2. **Loader**: Resolves addresses and creates executable binary
3. **MIPS Emulator**: Executes the final binary

### Benefits
- **Modularity**: Separate assembly and linking phases
- **Flexibility**: Same MERL can be loaded at different addresses
- **Reusability**: MERL modules can be combined
- **Educational**: Demonstrates real-world linking concepts

## Relocation Types

### Format 1: Simple Address Relocation
- **Purpose**: Fix absolute addresses in code
- **Process**: Add load address offset to specified memory location
- **Example**: `jal 0x00000000` → `jal 0x10001000`

### Future Formats
The loader is designed to support additional relocation formats:
- Relative addressing
- Symbol resolution
- Complex addressing modes

## Future Enhancements

### Planned Features
- **Symbol Resolution**: Support for external symbol references
- **Multiple Sections**: Handle data and code sections separately
- **Advanced Relocations**: Support for more complex addressing modes
- **Debug Information**: Preserve and process debug symbols

### Extensibility
The loader is designed to be easily extended:
- New relocation formats can be added
- Additional MERL sections can be supported
- Custom memory layouts can be implemented

## Troubleshooting

### Common Issues
1. **File Not Found**: Check MERL file path and permissions
2. **Invalid MERL Format**: Verify MERL magic number and structure
3. **Build Errors**: Ensure C++14 compiler and Make are available
4. **Output Issues**: Check stdout redirection

### Debug Mode
Enable debug output by modifying the loader to include verbose logging:
```cpp
cerr << "Debug: Processing relocation at address " << rel << endl;
```

## License

This project is part of a compiler construction course and is intended for educational purposes.

## Contributing

This is an educational project. For questions or issues, please refer to the course materials or instructor.
