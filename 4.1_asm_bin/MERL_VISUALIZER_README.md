# MERL File Visualizer

A Python script to visualize MERL (Module Entry Record Language) files as detailed tables with MIPS instruction disassembly.

## Features

- **Complete MERL Analysis**: Parses both simplified and full MERL formats
- **MIPS Disassembly**: Converts binary instructions back to assembly mnemonics
- **Table Visualization**: Displays MERL contents in organized tables
- **Markdown Export**: Generates Markdown tables for documentation
- **Command Line Interface**: Easy-to-use CLI with multiple options

## Files

- `merl_visualizer_final.py` - Main visualizer script
- `merl_visualizer.py` - Original version (backup)
- `merl_visualizer_v2.py` - Intermediate version (backup)

## Usage

### Basic Usage
```bash
python3 merl_visualizer_final.py <merl_file>
```

### Command Line Options
```bash
python3 merl_visualizer_final.py <merl_file> [options]

Options:
  --no-hex        Hide hexadecimal values
  --no-assembly   Hide assembly disassembly
  --export        Export to Markdown file
  --output FILE   Specify output file for Markdown export
```

### Examples

#### View MERL file in terminal
```bash
python3 merl_visualizer_final.py asm_test/m1.merl
```

#### Export to Markdown
```bash
python3 merl_visualizer_final.py asm_test/m1.merl --export
```

#### Export with custom filename
```bash
python3 merl_visualizer_final.py asm_test/m1.merl --export --output my_analysis.md
```

## Output Format

### Terminal Output
```
================================================================================
MERL File Visualization: m1.merl
================================================================================
Format: Simplified
Code Size: 48 bytes
Total Size: 45 bytes
Has Relocation Records: False
================================================================================
Offset   Hex Value    Type     Description          Assembly                      
----------------------------------------------------------------------------------
00000000 0x10000002   Header   MERL Magic Number   
00000004 0x00000030   Header   Code Size           
00000008 0x0000AFDF   Code     MIPS Instruction     unknown_r_type (funct=31)     
0000000C 0xFFFC3FA0   Code     MIPS Instruction     unknown_i_type (opcode=63)    
...
```

### Markdown Output
```markdown
# MERL File Analysis: m1.merl

## File Information

- **Format**: Simplified
- **Code Size**: 48 bytes
- **Total Size**: 45 bytes
- **Has Relocation Records**: False

## MERL File Contents

| Offset | Hex Value | Type | Description | Assembly |
|--------|-----------|------|-------------|----------|
| 0x00000000 | 0x10000002 | Header | MERL Magic Number |  |
| 0x00000004 | 0x00000030 | Header | Code Size |  |
| 0x00000008 | 0x0000AFDF | Code | MIPS Instruction | unknown_r_type (funct=31) |
...
```

## MERL File Format

### Simplified Format
```
[Magic Number][Code Size][Code Section][End Marker]
```

### Full Format
```
[Magic Number][End Mod][Code Size][Code Section][Relocation Records][End Marker]
```

### Header Information
- **Magic Number**: `0x10000002` (MERL identifier)
- **Code Size**: Size of the code section in bytes
- **End Mod**: End of module marker (full format only)

### Code Section
Contains compiled MIPS instructions and data words.

### End Marker
`0x10000001` marks the end of the MERL file.

## Supported MIPS Instructions

### R-Type Instructions (opcode = 0)
- `add`, `sub` - Arithmetic operations
- `mult`, `multu`, `div`, `divu` - Multiplication/division
- `mfhi`, `mflo` - Move from HI/LO registers
- `slt`, `sltu` - Set less than
- `jr`, `jalr` - Jump register

### I-Type Instructions
- `beq`, `bne` - Branch instructions
- `lis` - Load immediate and skip
- `lw`, `sw` - Load/store word

## Example Analysis

### m1.merl Analysis
- **Format**: Simplified MERL
- **Size**: 45 bytes total, 48 bytes code
- **Instructions**: Contains `sw`, `lis`, `jalr`, `lw`, `jr` instructions
- **Data**: Includes `.word` directives for labels

### m2.merl Analysis
- **Format**: Simplified MERL
- **Size**: 57 bytes total, 60 bytes code
- **Instructions**: Contains `lis`, `mult`, `jr` instructions
- **Data**: Includes `.word` directives for labels

## Integration with Assembler Pipeline

The visualizer works with the existing MIPS assembler pipeline:

1. **Assemble**: `./assembler/linkasm < input.asm > output.merl`
2. **Visualize**: `python3 merl_visualizer_final.py output.merl`
3. **Export**: `python3 merl_visualizer_final.py output.merl --export`

## Error Handling

The visualizer handles common errors:
- **File not found**: Clear error message
- **Invalid MERL format**: Magic number validation
- **Corrupted data**: Graceful handling of incomplete files
- **Unknown instructions**: Marked as "unknown" with opcode/funct info

## Dependencies

- Python 3.6+
- Standard library only (no external dependencies)

## Future Enhancements

- **Enhanced Disassembly**: Support for more MIPS instruction types
- **Relocation Analysis**: Parse and display relocation records
- **Symbol Table**: Extract and display symbol information
- **Graphical Interface**: GUI version with interactive features
- **Batch Processing**: Process multiple MERL files at once

## Troubleshooting

### Common Issues

1. **"File not found"**: Check file path and permissions
2. **"Invalid MERL magic number"**: File may be corrupted or not a MERL file
3. **"Unknown instructions"**: Normal for some instruction types not yet supported

### Debug Mode
For debugging, examine the raw binary data:
```bash
hexdump -C your_file.merl
```

## License

This tool is part of the CS241 assembler pipeline project and is intended for educational purposes.
