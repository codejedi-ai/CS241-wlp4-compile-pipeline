#!/usr/bin/env python3
"""
Final MERL File Visualizer
A Python script to visualize MERL files with accurate MIPS instruction disassembly.
"""

import struct
import sys
import os
from typing import List, Dict, Tuple, Optional
import argparse

class MERLVisualizer:
    """Visualizes MERL files in table format with detailed analysis."""
    
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
        
        # Read code size
        code_size = struct.unpack('>I', self.data[4:8])[0]
        
        # Check if file ends with MERL end marker
        has_end_marker = (len(self.data) >= 4 and 
                         self.data[-4:] == struct.pack('>I', self.MERL_END))
        
        if has_end_marker:
            format_type = "Simplified"
            has_relocation = False
        else:
            format_type = "Full"
            has_relocation = True
        
        return {
            'format_type': format_type,
            'code_size': code_size,
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
    
    def _analyze_entry(self, offset: int, value: int) -> Dict:
        """Analyze a MERL entry and determine its type and description."""
        if offset == 0:
            if value == self.MERL_MAGIC:
                return {'type': 'Header', 'description': 'MERL Magic Number', 'assembly': ''}
            else:
                return {'type': 'Header', 'description': f'Invalid Magic (0x{value:08X})', 'assembly': ''}
        
        elif offset == 4:
            return {'type': 'Header', 'description': 'Code Size', 'assembly': ''}
        
        elif offset >= 8:
            # Check if this is a MERL end marker
            if value == self.MERL_END:
                return {'type': 'Footer', 'description': 'MERL End Marker', 'assembly': ''}
            
            # Try to disassemble as instruction
            try:
                assembly = self._disassemble_instruction(value)
                return {'type': 'Code', 'description': 'MIPS Instruction', 'assembly': assembly}
            except:
                # If not a valid instruction, treat as data
                return {'type': 'Data', 'description': 'Data Word', 'assembly': f'.word {value}'}
        
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
            print(f"Code Size: {format_info['code_size']} bytes")
            print(f"Total Size: {format_info['total_size']} bytes")
            print(f"Has Relocation Records: {format_info['has_relocation']}")
            print(f"{'='*80}")
        except Exception as e:
            print(f"Error analyzing MERL format: {e}")
            return
        
        # Create table header
        header = f"{'Offset':<8} {'Hex Value':<12} {'Type':<8} {'Description':<20}"
        if show_assembly:
            header += f" {'Assembly':<30}"
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
                entry_info = self._analyze_entry(offset, value)
                
                # Format output
                hex_str = f"0x{value:08X}" if show_hex else f"{value:010d}"
                row = f"{offset:08X} {hex_str:<12} {entry_info['type']:<8} {entry_info['description']:<20}"
                
                if show_assembly and entry_info['assembly']:
                    row += f" {entry_info['assembly']:<30}"
                
                print(row)
                
                offset += 4
            else:
                # Handle remaining bytes
                remaining = len(self.data) - offset
                if remaining > 0:
                    print(f"{offset:08X} {'<incomplete>':<12} {'Padding':<8} {'Remaining bytes':<20}")
                break
    
    def export_to_markdown(self, output_file: str = None) -> None:
        """Export visualization to Markdown table format."""
        if output_file is None:
            output_file = f"{os.path.splitext(self.filename)[0]}_merl_table.md"
        
        with open(output_file, 'w') as f:
            f.write(f"# MERL File Analysis: {os.path.basename(self.filename)}\n\n")
            
            # Analyze file format
            try:
                format_info = self._analyze_merl_format()
                f.write(f"## File Information\n\n")
                f.write(f"- **Format**: {format_info['format_type']}\n")
                f.write(f"- **Code Size**: {format_info['code_size']} bytes\n")
                f.write(f"- **Total Size**: {format_info['total_size']} bytes\n")
                f.write(f"- **Has Relocation Records**: {format_info['has_relocation']}\n\n")
            except Exception as e:
                f.write(f"Error analyzing MERL format: {e}\n\n")
                return
            
            # Create markdown table
            f.write("## MERL File Contents\n\n")
            f.write("| Offset | Hex Value | Type | Description | Assembly |\n")
            f.write("|--------|-----------|------|-------------|----------|\n")
            
            # Parse and write entries
            offset = 0
            while offset < len(self.data):
                if offset + 4 <= len(self.data):
                    word_bytes = self.data[offset:offset+4]
                    value = struct.unpack('>I', word_bytes)[0]
                    
                    entry_info = self._analyze_entry(offset, value)
                    
                    f.write(f"| 0x{offset:08X} | 0x{value:08X} | {entry_info['type']} | {entry_info['description']} | {entry_info['assembly']} |\n")
                    
                    offset += 4
                else:
                    remaining = len(self.data) - offset
                    if remaining > 0:
                        f.write(f"| 0x{offset:08X} | <incomplete> | Padding | Remaining bytes | |\n")
                    break
        
        print(f"Markdown table exported to: {output_file}")

def main():
    """Main function to handle command line arguments and run visualization."""
    parser = argparse.ArgumentParser(description='Visualize MERL files as tables')
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
