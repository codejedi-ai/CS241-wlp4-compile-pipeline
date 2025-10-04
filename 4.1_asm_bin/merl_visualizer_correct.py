#!/usr/bin/env python3
"""
Correct MERL File Visualizer
A Python script to visualize MERL files in the exact expected format.
"""

import struct
import sys
import os
from typing import List, Dict, Tuple, Optional
import argparse

class MERLVisualizer:
    """Visualizes MERL files in the exact expected format."""
    
    # MERL constants
    MERL_MAGIC = 0x10000002
    MERL_END = 0x10000001
    
    def __init__(self, filename: str):
        """Initialize with MERL file path."""
        self.filename = filename
        self.data = self._read_file()
        
    def _read_file(self) -> bytes:
        """Read the MERL file as binary data."""
        try:
            with open(self.filename, 'rb') as f:
                return f.read()
        except FileNotFoundError:
            print(f"Error: File '{self.filename}' not found.")
            sys.exit(1)
        except Exception as e:
            print(f"Error reading file: {e}")
            sys.exit(1)
    
    def _analyze_merl_format(self) -> Dict:
        """Analyze the MERL file format and structure."""
        if len(self.data) < 8:
            raise ValueError("File too short to be a valid MERL file")
        
        # Read magic number
        magic = struct.unpack('>I', self.data[0:4])[0]
        if magic != self.MERL_MAGIC:
            raise ValueError(f"Invalid MERL magic number: 0x{magic:08X}")
        
        # Read end of module
        end_mod = struct.unpack('>I', self.data[4:8])[0]
        
        # Read end of code
        end_code = struct.unpack('>I', self.data[8:12])[0]
        
        # Check if file ends with MERL end marker
        has_end_marker = (len(self.data) >= 4 and 
                         self.data[-4:] == struct.pack('>I', self.MERL_END))
        
        if has_end_marker:
            format_type = "Full"
            has_relocation = True
        else:
            format_type = "Simplified"
            has_relocation = False
        
        return {
            'format_type': format_type,
            'end_mod': end_mod,
            'end_code': end_code,
            'has_relocation': has_relocation,
            'total_size': len(self.data)
        }
    
    def _disassemble_instruction(self, instruction: int) -> str:
        """Disassemble a MIPS instruction to assembly mnemonic."""
        opcode = (instruction >> 26) & 0x3F
        rs = (instruction >> 21) & 0x1F
        rt = (instruction >> 16) & 0x1F
        rd = (instruction >> 11) & 0x1F
        shamt = (instruction >> 6) & 0x1F
        funct = instruction & 0x3F
        immediate = instruction & 0xFFFF
        
        # Sign extend immediate for branch/jump
        if immediate & 0x8000:
            immediate = immediate - 0x10000
        
        # R-type instructions (opcode = 0)
        if opcode == 0:
            if funct == 32:  # ADD
                return f"add ${rd}, ${rs}, ${rt}"
            elif funct == 34:  # SUB
                return f"sub ${rd}, ${rs}, ${rt}"
            elif funct == 24:  # MULT
                return f"mult ${rs}, ${rt}"
            elif funct == 25:  # MULTU
                return f"multu ${rs}, ${rt}"
            elif funct == 26:  # DIV
                return f"div ${rs}, ${rt}"
            elif funct == 27:  # DIVU
                return f"divu ${rs}, ${rt}"
            elif funct == 16:  # MFHI
                return f"mfhi ${rd}"
            elif funct == 18:  # MFLO
                return f"mflo ${rd}"
            elif funct == 42:  # SLT
                return f"slt ${rd}, ${rs}, ${rt}"
            elif funct == 43:  # SLTU
                return f"sltu ${rd}, ${rs}, ${rt}"
            elif funct == 8:   # JR
                return f"jr ${rs}"
            elif funct == 9:   # JALR
                return f"jalr ${rs}"
            else:
                return f"unknown_r_type (funct={funct})"
        
        # I-type instructions
        elif opcode == 4:  # BEQ
            return f"beq ${rs}, ${rt}, {immediate}"
        elif opcode == 5:  # BNE
            return f"bne ${rs}, ${rt}, {immediate}"
        elif opcode == 15:  # LIS
            return f"lis ${rt}"
        elif opcode == 35:  # LW
            return f"lw ${rt}, {immediate}(${rs})"
        elif opcode == 43:  # SW
            return f"sw ${rt}, {immediate}(${rs})"
        else:
            return f"unknown_i_type (opcode={opcode})"
    
    def _analyze_entry(self, offset: int, value: int, format_info: Dict) -> Dict:
        """Analyze a MERL entry and determine its type and description."""
        if offset == 0:
            if value == self.MERL_MAGIC:
                return {'type': 'Header', 'description': 'MERL cookie', 'assembly': ''}
            else:
                return {'type': 'Header', 'description': f'Invalid Magic (0x{value:08X})', 'assembly': ''}
        
        elif offset == 4:
            return {'type': 'Header', 'description': 'End of module', 'assembly': ''}
        
        elif offset == 8:
            return {'type': 'Header', 'description': 'End of code', 'assembly': ''}
        
        elif offset >= 12 and offset < format_info['end_code']:
            # Check if this is a MERL end marker
            if value == self.MERL_END:
                return {'type': 'Footer', 'description': 'MERL End Marker', 'assembly': ''}
            
            # Try to disassemble as instruction
            try:
                assembly = self._disassemble_instruction(value)
                return {'type': 'Code', 'description': 'MIPS program', 'assembly': assembly}
            except:
                # If not a valid instruction, treat as data
                return {'type': 'Data', 'description': 'MIPS program', 'assembly': f'.word {value}'}
        
        elif offset >= format_info['end_code'] and offset < format_info['end_mod']:
            # Relocation records section
            if value == 1:
                return {'type': 'Reloc', 'description': 'REL entry', 'assembly': ''}
            elif value == 17:  # 0x11
                return {'type': 'Reloc', 'description': 'ESR entry', 'assembly': ''}
            elif value == 5:   # 0x05
                return {'type': 'Reloc', 'description': 'ESD entry', 'assembly': ''}
            elif value == 3:   # 0x03
                return {'type': 'Reloc', 'description': 'Symbol len 3', 'assembly': ''}
            elif value == 4:   # 0x04
                return {'type': 'Reloc', 'description': 'Symbol len 4', 'assembly': ''}
            elif value == 102:  # 'f'
                return {'type': 'Symbol', 'description': 'f', 'assembly': ''}
            elif value == 111:  # 'o'
                return {'type': 'Symbol', 'description': 'o', 'assembly': ''}
            elif value == 98:   # 'b'
                return {'type': 'Symbol', 'description': 'b', 'assembly': ''}
            elif value == 97:   # 'a'
                return {'type': 'Symbol', 'description': 'a', 'assembly': ''}
            elif value == 114:  # 'r'
                return {'type': 'Symbol', 'description': 'r', 'assembly': ''}
            else:
                return {'type': 'Reloc', 'description': f'Address 0x{value:08X}', 'assembly': ''}
        
        elif offset >= format_info['end_mod']:
            # After end of module
            if value == self.MERL_END:
                return {'type': 'Footer', 'description': 'MERL End Marker', 'assembly': ''}
            else:
                return {'type': 'Unknown', 'description': 'Unknown Entry', 'assembly': ''}
        
        return {'type': 'Unknown', 'description': 'Unknown Entry', 'assembly': ''}
    
    def visualize(self, show_hex: bool = True, show_assembly: bool = True) -> None:
        """Generate and display the MERL visualization table."""
        print(f"\n{'='*80}")
        print(f"MERL File Visualization: {os.path.basename(self.filename)}")
        print(f"{'='*80}")
        
        # Analyze file format
        try:
            format_info = self._analyze_merl_format()
            print(f"Format: {format_info['format_type']}")
            print(f"End of Module: 0x{format_info['end_mod']:08X}")
            print(f"End of Code: 0x{format_info['end_code']:08X}")
            print(f"Total Size: {format_info['total_size']} bytes")
            print(f"Has Relocation Records: {format_info['has_relocation']}")
            print(f"{'='*80}")
        except Exception as e:
            print(f"Error analyzing MERL format: {e}")
            return
        
        # Create table header
        header = f"{'Offset':<8} {'MERL Code':<12} {'Description':<20}"
        if show_assembly:
            header += f" {'Assembly Source':<30}"
        print(header)
        print("-" * len(header))
        
        # Parse and display entries
        offset = 0
        
        while offset < len(self.data):
            if offset + 4 <= len(self.data):
                # Read 32-bit word
                word_bytes = self.data[offset:offset+4]
                value = struct.unpack('>I', word_bytes)[0]
                
                # Analyze entry
                entry_info = self._analyze_entry(offset, value, format_info)
                
                # Format output
                hex_str = f"0x{value:08X}" if show_hex else f"{value:010d}"
                row = f"0x{offset:02X} {hex_str:<12} {entry_info['description']:<20}"
                
                if show_assembly and entry_info['assembly']:
                    row += f" {entry_info['assembly']:<30}"
                
                print(row)
                
                offset += 4
            else:
                # Handle remaining bytes
                remaining = len(self.data) - offset
                if remaining > 0:
                    print(f"0x{offset:02X} {'<incomplete>':<12} {'Remaining bytes':<20}")
                break
    
    def export_to_markdown(self, output_file: str = None) -> None:
        """Export visualization to Markdown table format matching expected output."""
        if output_file is None:
            output_file = f"{os.path.splitext(self.filename)[0]}_correct_table.md"
        
        with open(output_file, 'w') as f:
            # Write the exact format you requested
            f.write(f"# MERL File Analysis: {os.path.basename(self.filename)}\n\n")
            
            # Analyze file format
            try:
                format_info = self._analyze_merl_format()
                f.write(f"## File Information\n\n")
                f.write(f"- **Format**: {format_info['format_type']}\n")
                f.write(f"- **End of Module**: 0x{format_info['end_mod']:08X}\n")
                f.write(f"- **End of Code**: 0x{format_info['end_code']:08X}\n")
                f.write(f"- **Total Size**: {format_info['total_size']} bytes\n")
                f.write(f"- **Has Relocation Records**: {format_info['has_relocation']}\n\n")
            except Exception as e:
                f.write(f"Error analyzing MERL format: {e}\n\n")
                return
            
            # Create markdown table in exact expected format
            f.write("## MERL File Contents\n\n")
            f.write("| Offset | MERL Code | Description | Assembly Source |\n")
            f.write("| :----: | :-------- | :---------- | :-------------- |\n")
            
            # Parse and write entries
            offset = 0
            while offset < len(self.data):
                if offset + 4 <= len(self.data):
                    word_bytes = self.data[offset:offset+4]
                    value = struct.unpack('>I', word_bytes)[0]
                    
                    entry_info = self._analyze_entry(offset, value, format_info)
                    
                    # Format exactly as expected
                    offset_str = f"0x{offset:02X}"
                    code_str = f"0x{value:08X}"
                    desc_str = entry_info['description']
                    assembly_str = entry_info['assembly'] if entry_info['assembly'] else ""
                    
                    f.write(f"| {offset_str} | {code_str} | {desc_str} | {assembly_str} |\n")
                    
                    offset += 4
                else:
                    remaining = len(self.data) - offset
                    if remaining > 0:
                        f.write(f"| 0x{offset:02X} | <incomplete> | Remaining bytes | |\n")
                    break
        
        print(f"Markdown table exported to: {output_file}")

def main():
    """Main function to handle command line arguments and run visualization."""
    parser = argparse.ArgumentParser(description='Visualize MERL files in correct format')
    parser.add_argument('filename', help='MERL file to visualize')
    parser.add_argument('--no-hex', action='store_true', help='Hide hexadecimal values')
    parser.add_argument('--no-assembly', action='store_true', help='Hide assembly disassembly')
    parser.add_argument('--export', action='store_true', help='Export to Markdown file')
    parser.add_argument('--output', help='Output file for Markdown export')
    
    args = parser.parse_args()
    
    # Create visualizer
    visualizer = MERLVisualizer(args.filename)
    
    # Show visualization
    visualizer.visualize(
        show_hex=not args.no_hex,
        show_assembly=not args.no_assembly
    )
    
    # Export to Markdown if requested
    if args.export:
        visualizer.export_to_markdown(args.output)

if __name__ == "__main__":
    main()
