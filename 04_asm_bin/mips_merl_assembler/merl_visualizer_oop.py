#!/usr/bin/env python3
"""
OOP MERL File Visualizer
A Python script that mirrors the C++ OOP instruction structure for MERL file visualization.
"""

import struct
import sys
import os
from typing import List, Dict, Tuple, Optional, Union
import argparse
from abc import ABC, abstractmethod

# MERL Constants (matching C++ version)
MERL_MAGIC = 0x10000002
MERL_END = 0x10000001
REL_TYPE = 0x00000001
ESR_TYPE = 0x00000011
ESD_TYPE = 0x00000005

class MIPSInstruction(ABC):
    """Base class for all MIPS instructions."""
    
    @abstractmethod
    def encode(self) -> int:
        """Return the encoded instruction as a 32-bit integer."""
        pass
    
    @abstractmethod
    def get_mnemonic(self) -> str:
        """Return the instruction mnemonic."""
        pass
    
    @abstractmethod
    def get_description(self) -> str:
        """Return a description of the instruction."""
        pass
    
    def get_instruction_value(self) -> int:
        """Get the instruction as uint32_t for array storage."""
        return self.encode()

class ITypeInstruction(MIPSInstruction):
    """Base class for I-Type instructions."""
    
    def __init__(self, opcode: int, rs: int, rt: int, immediate: int):
        self.opcode = opcode
        self.rs = rs
        self.rt = rt
        self.immediate = immediate
    
    def encode(self) -> int:
        return (self.opcode << 26) | (self.rs << 21) | (self.rt << 16) | (self.immediate & 0xFFFF)

class RTypeInstruction(MIPSInstruction):
    """Base class for R-Type instructions."""
    
    def __init__(self, rs: int, rt: int, rd: int, shamt: int, funct: int):
        self.rs = rs
        self.rt = rt
        self.rd = rd
        self.shamt = shamt
        self.funct = funct
    
    def encode(self) -> int:
        return (self.rs << 21) | (self.rt << 16) | (self.rd << 11) | (self.shamt << 6) | self.funct

# I-Type Instructions
class LoadWordInstruction(ITypeInstruction):
    def __init__(self, rt: int, immediate: int, rs: int):
        super().__init__(0x23, rs, rt, immediate)  # 0x23 = 35 decimal
    
    def get_mnemonic(self) -> str:
        return "lw"
    
    def get_description(self) -> str:
        return "Load Word"

class StoreWordInstruction(ITypeInstruction):
    def __init__(self, rt: int, immediate: int, rs: int):
        super().__init__(0x2B, rs, rt, immediate)  # 0x2B = 43 decimal
    
    def get_mnemonic(self) -> str:
        return "sw"
    
    def get_description(self) -> str:
        return "Store Word"

class BranchEqualInstruction(ITypeInstruction):
    def __init__(self, rs: int, rt: int, immediate: int):
        super().__init__(0x04, rs, rt, immediate)  # 0x04 = 4 decimal
    
    def get_mnemonic(self) -> str:
        return "beq"
    
    def get_description(self) -> str:
        return "Branch On Equal"

class BranchNotEqualInstruction(ITypeInstruction):
    def __init__(self, rs: int, rt: int, immediate: int):
        super().__init__(0x05, rs, rt, immediate)  # 0x05 = 5 decimal
    
    def get_mnemonic(self) -> str:
        return "bne"
    
    def get_description(self) -> str:
        return "Branch On Not Equal"

class AddImmediateInstruction(ITypeInstruction):
    def __init__(self, rt: int, rs: int, immediate: int):
        super().__init__(0x08, rs, rt, immediate)  # 0x08 = 8 decimal
    
    def get_mnemonic(self) -> str:
        return "addi"
    
    def get_description(self) -> str:
        return "Add Immediate"

class SetLessThanImmediateInstruction(ITypeInstruction):
    def __init__(self, rt: int, rs: int, immediate: int):
        super().__init__(0x0A, rs, rt, immediate)  # 0x0A = 10 decimal
    
    def get_mnemonic(self) -> str:
        return "slti"
    
    def get_description(self) -> str:
        return "Set Less Than Immediate"

class SetLessThanImmediateUnsignedInstruction(ITypeInstruction):
    def __init__(self, rt: int, rs: int, immediate: int):
        super().__init__(0x0B, rs, rt, immediate)  # 0x0B = 11 decimal
    
    def get_mnemonic(self) -> str:
        return "sltiu"
    
    def get_description(self) -> str:
        return "Set Less Than Immediate Unsigned"

# R-Type Instructions
class AddInstruction(RTypeInstruction):
    def __init__(self, rd: int, rs: int, rt: int):
        super().__init__(rs, rt, rd, 0, 0x20)  # funct = 0x20 = 32 decimal
    
    def get_mnemonic(self) -> str:
        return "add"
    
    def get_description(self) -> str:
        return "Add"

class SubtractInstruction(RTypeInstruction):
    def __init__(self, rd: int, rs: int, rt: int):
        super().__init__(rs, rt, rd, 0, 0x22)  # funct = 0x22 = 34 decimal
    
    def get_mnemonic(self) -> str:
        return "sub"
    
    def get_description(self) -> str:
        return "Subtract"

class SetLessThanInstruction(RTypeInstruction):
    def __init__(self, rd: int, rs: int, rt: int):
        super().__init__(rs, rt, rd, 0, 0x2A)  # funct = 0x2A = 42 decimal
    
    def get_mnemonic(self) -> str:
        return "slt"
    
    def get_description(self) -> str:
        return "Set Less Than"

class SetLessThanUnsignedInstruction(RTypeInstruction):
    def __init__(self, rd: int, rs: int, rt: int):
        super().__init__(rs, rt, rd, 0, 0x2B)  # funct = 0x2B = 43 decimal
    
    def get_mnemonic(self) -> str:
        return "sltu"
    
    def get_description(self) -> str:
        return "Set Less Than Unsigned"

class MultiplyInstruction(RTypeInstruction):
    def __init__(self, rs: int, rt: int):
        super().__init__(rs, rt, 0, 0, 0x18)  # funct = 0x18 = 24 decimal
    
    def get_mnemonic(self) -> str:
        return "mult"
    
    def get_description(self) -> str:
        return "Multiply"

class MultiplyUnsignedInstruction(RTypeInstruction):
    def __init__(self, rs: int, rt: int):
        super().__init__(rs, rt, 0, 0, 0x19)  # funct = 0x19 = 25 decimal
    
    def get_mnemonic(self) -> str:
        return "multu"
    
    def get_description(self) -> str:
        return "Multiply Unsigned"

class DivideInstruction(RTypeInstruction):
    def __init__(self, rs: int, rt: int):
        super().__init__(rs, rt, 0, 0, 0x1A)  # funct = 0x1A = 26 decimal
    
    def get_mnemonic(self) -> str:
        return "div"
    
    def get_description(self) -> str:
        return "Divide"

class DivideUnsignedInstruction(RTypeInstruction):
    def __init__(self, rs: int, rt: int):
        super().__init__(rs, rt, 0, 0, 0x1B)  # funct = 0x1B = 27 decimal
    
    def get_mnemonic(self) -> str:
        return "divu"
    
    def get_description(self) -> str:
        return "Divide Unsigned"

class MoveFromHighInstruction(RTypeInstruction):
    def __init__(self, rd: int):
        super().__init__(0, 0, rd, 0, 0x10)  # funct = 0x10 = 16 decimal
    
    def get_mnemonic(self) -> str:
        return "mfhi"
    
    def get_description(self) -> str:
        return "Move From High"

class MoveFromLowInstruction(RTypeInstruction):
    def __init__(self, rd: int):
        super().__init__(0, 0, rd, 0, 0x12)  # funct = 0x12 = 18 decimal
    
    def get_mnemonic(self) -> str:
        return "mflo"
    
    def get_description(self) -> str:
        return "Move From Low"

class LoadImmediateAndSkipInstruction(RTypeInstruction):
    def __init__(self, rd: int):
        super().__init__(0, 0, rd, 0, 0x14)  # funct = 0x14 = 20 decimal
    
    def get_mnemonic(self) -> str:
        return "lis"
    
    def get_description(self) -> str:
        return "Load Immediate And Skip"

class JumpRegisterInstruction(RTypeInstruction):
    def __init__(self, rs: int):
        super().__init__(rs, 0, 0, 0, 0x08)  # funct = 0x08 = 8 decimal
    
    def get_mnemonic(self) -> str:
        return "jr"
    
    def get_description(self) -> str:
        return "Jump Register"

class JumpAndLinkRegisterInstruction(RTypeInstruction):
    def __init__(self, rs: int):
        super().__init__(rs, 0, 0, 0, 0x09)  # funct = 0x09 = 9 decimal
    
    def get_mnemonic(self) -> str:
        return "jalr"
    
    def get_description(self) -> str:
        return "Jump And Link Register"

class WordDirective(MIPSInstruction):
    """Word directive (not a real instruction, but handled similarly)."""
    
    def __init__(self, value: int):
        self.value = value
    
    def encode(self) -> int:
        return self.value
    
    def get_mnemonic(self) -> str:
        return ".word"
    
    def get_description(self) -> str:
        return "Word Directive"

class InstructionFactory:
    """Factory for creating instruction objects from encoded values."""
    
    @staticmethod
    def create_instruction_from_encoded(encoded_value: int) -> MIPSInstruction:
        """Create an instruction object from an encoded 32-bit value."""
        opcode = (encoded_value >> 26) & 0x3F
        
        if opcode == 0:  # R-Type instruction
            rs = (encoded_value >> 21) & 0x1F
            rt = (encoded_value >> 16) & 0x1F
            rd = (encoded_value >> 11) & 0x1F
            shamt = (encoded_value >> 6) & 0x1F
            funct = encoded_value & 0x3F
            
            if funct == 0x20:  # ADD
                return AddInstruction(rd, rs, rt)
            elif funct == 0x22:  # SUB
                return SubtractInstruction(rd, rs, rt)
            elif funct == 0x2A:  # SLT
                return SetLessThanInstruction(rd, rs, rt)
            elif funct == 0x2B:  # SLTU
                return SetLessThanUnsignedInstruction(rd, rs, rt)
            elif funct == 0x18:  # MULT
                return MultiplyInstruction(rs, rt)
            elif funct == 0x19:  # MULTU
                return MultiplyUnsignedInstruction(rs, rt)
            elif funct == 0x1A:  # DIV
                return DivideInstruction(rs, rt)
            elif funct == 0x1B:  # DIVU
                return DivideUnsignedInstruction(rs, rt)
            elif funct == 0x10:  # MFHI
                return MoveFromHighInstruction(rd)
            elif funct == 0x12:  # MFLO
                return MoveFromLowInstruction(rd)
            elif funct == 0x14:  # LIS
                return LoadImmediateAndSkipInstruction(rd)
            elif funct == 0x08:  # JR
                return JumpRegisterInstruction(rs)
            elif funct == 0x09:  # JALR
                return JumpAndLinkRegisterInstruction(rs)
            else:
                # Unknown R-type instruction
                return WordDirective(encoded_value)
        
        else:  # I-Type instruction
            rs = (encoded_value >> 21) & 0x1F
            rt = (encoded_value >> 16) & 0x1F
            immediate = encoded_value & 0xFFFF
            
            if opcode == 0x23:  # LW
                return LoadWordInstruction(rt, immediate, rs)
            elif opcode == 0x2B:  # SW
                return StoreWordInstruction(rt, immediate, rs)
            elif opcode == 0x04:  # BEQ
                return BranchEqualInstruction(rs, rt, immediate)
            elif opcode == 0x05:  # BNE
                return BranchNotEqualInstruction(rs, rt, immediate)
            elif opcode == 0x08:  # ADDI
                return AddImmediateInstruction(rt, rs, immediate)
            elif opcode == 0x0A:  # SLTI
                return SetLessThanImmediateInstruction(rt, rs, immediate)
            elif opcode == 0x0B:  # SLTIU
                return SetLessThanImmediateUnsignedInstruction(rt, rs, immediate)
            else:
                # Unknown I-type instruction
                return WordDirective(encoded_value)

class MERLVisualizer:
    """Visualizes MERL files using OOP instruction classes."""
    
    def __init__(self, filename: str):
        """Initialize with MERL file path."""
        self.filename = filename
        self.data = self._read_file()
        self.words = self._parse_as_word_array()
        self.pos = 0
        
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
    
    def _parse_as_word_array(self) -> List[int]:
        """Parse the binary data as an array of 32-bit words (big-endian)."""
        words = []
        for i in range(0, len(self.data), 4):
            if i + 4 <= len(self.data):
                word = struct.unpack('>I', self.data[i:i+4])[0]
                words.append(word)
        return words
    
    def _read_word(self) -> int:
        """Read a 32-bit word from current position."""
        if self.pos >= len(self.words):
            raise ValueError("Unexpected end of file while reading word")
        
        word = self.words[self.pos]
        self.pos += 1
        return word
    
    def _analyze_merl_format(self) -> Dict:
        """Analyze the MERL file format and structure."""
        self.pos = 0
        
        try:
            # Read header
            magic = self._read_word()
            if magic != MERL_MAGIC:
                raise ValueError(f"Invalid MERL magic number: 0x{magic:08X}")
            
            end_of_module = self._read_word()
            end_of_code = self._read_word()
            
            code_size = end_of_code - 12  # Subtract header size
            reloc_table_size = end_of_module - end_of_code - 4  # Subtract MERL_END
            
            return {
                'magic': magic,
                'end_of_module': end_of_module,
                'end_of_code': end_of_code,
                'code_size': code_size,
                'reloc_table_size': reloc_table_size,
                'total_size': len(self.words) * 4
            }
        except Exception as e:
            raise ValueError(f"Error analyzing MERL format: {e}")
    
    def _disassemble_instruction(self, word: int) -> Tuple[str, str]:
        """Disassemble a MIPS instruction using OOP classes."""
        try:
            instruction = InstructionFactory.create_instruction_from_encoded(word)
            mnemonic = instruction.get_mnemonic()
            description = instruction.get_description()
            
            # Format the instruction with operands
            if mnemonic == "lw":
                rt = (word >> 16) & 0x1F
                rs = (word >> 21) & 0x1F
                immediate = word & 0xFFFF
                if immediate >= 0x8000:  # Sign extend
                    immediate -= 0x10000
                return f"{mnemonic} ${rt}, {immediate}(${rs})", description
            elif mnemonic == "sw":
                rt = (word >> 16) & 0x1F
                rs = (word >> 21) & 0x1F
                immediate = word & 0xFFFF
                if immediate >= 0x8000:  # Sign extend
                    immediate -= 0x10000
                return f"{mnemonic} ${rt}, {immediate}(${rs})", description
            elif mnemonic == "lis":
                rd = (word >> 11) & 0x1F
                return f"{mnemonic} ${rd}", description
            elif mnemonic == "jr":
                rs = (word >> 21) & 0x1F
                return f"{mnemonic} ${rs}", description
            elif mnemonic == "jalr":
                rs = (word >> 21) & 0x1F
                return f"{mnemonic} ${rs}", description
            elif mnemonic == ".word":
                return f"{mnemonic} 0x{word:08X}", description
            else:
                return f"{mnemonic} (unknown format)", description
                
        except Exception:
            return f"unknown_instruction 0x{word:08X}", "Unknown Instruction"
    
    def _parse_relocation_entries(self, start_pos: int, end_pos: int) -> List[Dict]:
        """Parse relocation entries from the MERL file."""
        entries = []
        pos = start_pos
        
        while pos < end_pos:
            if pos >= len(self.words):
                break
                
            entry_type = self.words[pos]
            
            if entry_type == REL_TYPE:
                pos += 1
                if pos < len(self.words):
                    address = self.words[pos]
                    entries.append({
                        'type': 'REL',
                        'address': address,
                        'length': 0,
                        'name': f'rel_{address:08X}'
                    })
                    pos += 1
            
            elif entry_type == ESR_TYPE:
                pos += 1
                if pos + 2 < len(self.words):
                    address = self.words[pos]
                    length = self.words[pos + 1]
                    name = ""
                    pos += 2
                    for i in range(length):
                        if pos < len(self.words):
                            name += chr(self.words[pos] & 0xFF)
                            pos += 1
                    entries.append({
                        'type': 'ESR',
                        'address': address,
                        'length': length,
                        'name': name
                    })
            
            elif entry_type == ESD_TYPE:
                pos += 1
                if pos + 2 < len(self.words):
                    address = self.words[pos]
                    length = self.words[pos + 1]
                    name = ""
                    pos += 2
                    for i in range(length):
                        if pos < len(self.words):
                            name += chr(self.words[pos] & 0xFF)
                            pos += 1
                    entries.append({
                        'type': 'ESD',
                        'address': address,
                        'length': length,
                        'name': name
                    })
            
            elif entry_type == MERL_END:
                break
            
            else:
                pos += 1
        
        return entries
    
    def visualize(self, show_hex: bool = True, show_assembly: bool = True, 
                  show_export: bool = False, output_file: Optional[str] = None):
        """Visualize the MERL file with detailed analysis."""
        try:
            analysis = self._analyze_merl_format()
            
            output_lines = []
            output_lines.append("=" * 80)
            output_lines.append(f"MERL File Visualization: {self.filename}")
            output_lines.append("=" * 80)
            output_lines.append(f"Format: OOP Instruction Classes")
            output_lines.append(f"Code Size: {analysis['code_size']} bytes")
            output_lines.append(f"Total Size: {analysis['total_size']} bytes")
            output_lines.append(f"Has Relocation Records: {analysis['reloc_table_size'] > 0}")
            output_lines.append("=" * 80)
            
            if show_hex or show_assembly:
                output_lines.append("Offset   Hex Value    Type     Description          Assembly")
                output_lines.append("-" * 80)
            
            # Reset position and skip header
            self.pos = 3  # Skip magic, end_of_module, end_of_code
            
            # Process code section
            code_end = analysis['end_of_code'] // 4
            while self.pos < code_end and self.pos < len(self.words):
                offset = self.pos * 4
                word = self.words[self.pos]
                
                line_parts = []
                if show_hex:
                    line_parts.append(f"{offset:08X}  0x{word:08X}")
                else:
                    line_parts.append(f"{offset:08X}")
                
                # Determine if this is an instruction or data
                if offset >= 12:  # After header
                    assembly, description = self._disassemble_instruction(word)
                    line_parts.extend([
                        f"{'Code':<10}",
                        f"{description:<20}",
                        f"{assembly:<20}"
                    ])
                else:
                    line_parts.extend([
                        f"{'Header':<10}",
                        f"{'MERL Header':<20}",
                        f"{'':<20}"
                    ])
                
                output_lines.append("  ".join(line_parts))
                self.pos += 1
            
            # Process relocation entries
            if analysis['reloc_table_size'] > 0:
                entries = self._parse_relocation_entries(self.pos, len(self.words))
                
                output_lines.append("")
                output_lines.append("Relocation Entries:")
                output_lines.append("-" * 50)
                
                for entry in entries:
                    output_lines.append(f"{entry['type']:<4} 0x{entry['address']:08X} "
                                      f"Length: {entry['length']} Name: {entry['name']}")
            
            # Output results
            result = "\n".join(output_lines)
            
            if output_file:
                with open(output_file, 'w') as f:
                    f.write(result)
                print(f"Visualization saved to {output_file}")
            else:
                print(result)
                
        except Exception as e:
            print(f"Error during visualization: {e}")
            sys.exit(1)

def main():
    """Main function to handle command line arguments and run visualization."""
    parser = argparse.ArgumentParser(description='OOP MERL File Visualizer')
    parser.add_argument('filename', help='MERL file to visualize')
    parser.add_argument('--no-hex', action='store_true', help='Hide hex values')
    parser.add_argument('--no-assembly', action='store_true', help='Hide assembly disassembly')
    parser.add_argument('--export', action='store_true', help='Show export information')
    parser.add_argument('--output', '-o', help='Output file (default: stdout)')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.filename):
        print(f"Error: File '{args.filename}' not found.")
        sys.exit(1)
    
    visualizer = MERLVisualizer(args.filename)
    visualizer.visualize(
        show_hex=not args.no_hex,
        show_assembly=not args.no_assembly,
        show_export=args.export,
        output_file=args.output
    )

if __name__ == "__main__":
    main()
