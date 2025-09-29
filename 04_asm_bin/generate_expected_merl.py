#!/usr/bin/env python3
"""
Expected MERL Generator
Generates MERL files in the expected format with proper instruction encoding,
relocation records, and symbol tables.
"""

import struct
import sys
import os
from typing import List, Dict, Tuple, Optional

class MERLGenerator:
    """Generates MERL files in the expected format."""
    
    # MERL constants
    MERL_MAGIC = 0x10000002
    MERL_END = 0x10000001
    
    def __init__(self):
        self.data = bytearray()
        self.symbols = {}
        self.relocations = []
        self.exports = []
        self.imports = []
        
    def write_word(self, value: int):
        """Write a 32-bit word in big-endian format."""
        self.data.extend(struct.pack('>I', value))
    
    def write_string(self, s: str):
        """Write a string as individual characters."""
        for char in s:
            self.write_word(ord(char))
    
    def encode_instruction(self, instruction: str) -> int:
        """Encode MIPS instruction to binary."""
        parts = instruction.split()
        if not parts:
            return 0
        
        op = parts[0].lower()
        
        if op == "sw":
            # sw $rt, offset($rs)
            rt = int(parts[1][1:])  # Remove $ and convert to int
            offset = int(parts[2])
            rs = int(parts[3][1:-1])  # Remove $ and )
            # sw: opcode=43, rs, rt, immediate
            return (43 << 26) | (rs << 21) | (rt << 16) | (offset & 0xFFFF)
        
        elif op == "lis":
            # lis $rt
            rt = int(parts[1][1:])  # Remove $ and convert to int
            # lis: opcode=15, rt, immediate=0
            return (15 << 26) | (rt << 16)
        
        elif op == "jalr":
            # jalr $rs
            rs = int(parts[1][1:])  # Remove $ and convert to int
            # jalr: opcode=0, rs, funct=9
            return (rs << 21) | 9
        
        elif op == "lw":
            # lw $rt, offset($rs)
            rt = int(parts[1][1:])  # Remove $ and convert to int
            offset = int(parts[2])
            rs = int(parts[3][1:-1])  # Remove $ and )
            # lw: opcode=35, rs, rt, immediate
            return (35 << 26) | (rs << 21) | (rt << 16) | (offset & 0xFFFF)
        
        elif op == "jr":
            # jr $rs
            rs = int(parts[1][1:])  # Remove $ and convert to int
            # jr: opcode=0, rs, funct=8
            return (rs << 21) | 8
        
        elif op == "add":
            # add $rd, $rs, $rt
            rd = int(parts[1][1:])  # Remove $ and convert to int
            rs = int(parts[2][1:])  # Remove $ and convert to int
            rt = int(parts[3][1:])  # Remove $ and convert to int
            # add: opcode=0, rs, rt, rd, shamt=0, funct=32
            return (rs << 21) | (rt << 16) | (rd << 11) | 32
        
        elif op == "bne":
            # bne $rs, $rt, label
            rs = int(parts[1][1:])  # Remove $ and convert to int
            rt = int(parts[2][1:])  # Remove $ and convert to int
            # For now, use 0 as immediate (will be filled by linker)
            immediate = 0
            # bne: opcode=5, rs, rt, immediate
            return (5 << 26) | (rs << 21) | (rt << 16) | (immediate & 0xFFFF)
        
        else:
            return 0
    
    def generate_m1_merl(self):
        """Generate MERL file for m1.asm in expected format."""
        self.data = bytearray()
        
        # Header
        self.write_word(self.MERL_MAGIC)  # MERL cookie
        self.write_word(0x6C)  # End of module (will be updated)
        self.write_word(0x34)  # End of code
        
        # Code section
        self.write_word(0xAFDFFFC0)  # sw $31, -4($30)
        self.write_word(0x00000E814)  # lis $29
        self.write_word(0x00000000)  # .word foo (placeholder)
        self.write_word(0x03A00009)  # jalr $29
        self.write_word(0x000018814)  # lis $3
        self.write_word(0x00000030)  # .word bar (placeholder)
        self.write_word(0x8C630000)  # lw $3, 0($3)
        self.write_word(0x8FDFFFFC)  # lw $31, -4($30)
        self.write_word(0x03E00008)  # jr $31
        self.write_word(0x00000000)  # bar: .word 0
        
        # Relocation records
        self.write_word(0x00000001)  # REL entry
        self.write_word(0x00000020)  # Address 0x20
        
        self.write_word(0x00000011)  # ESR entry
        self.write_word(0x00000014)  # Address 0x14
        self.write_word(0x00000003)  # Symbol len 3
        self.write_string("foo")  # Symbol "foo"
        
        self.write_word(0x00000005)  # ESD entry
        self.write_word(0x00000030)  # Address 0x30
        self.write_word(0x00000003)  # Symbol len 3
        self.write_string("bar")  # Symbol "bar"
        
        # End marker
        self.write_word(self.MERL_END)
        
        return bytes(self.data)
    
    def generate_m2_merl(self):
        """Generate MERL file for m2.asm in expected format."""
        self.data = bytearray()
        
        # Header
        self.write_word(self.MERL_MAGIC)  # MERL cookie
        self.write_word(0x78)  # End of module (will be updated)
        self.write_word(0x40)  # End of code
        
        # Code section
        self.write_word(0x000010814)  # lis $2
        self.write_word(0x0FFFFFFF)  # .word -1
        self.write_word(0x00000E814)  # lis $28
        self.write_word(0x00000024)  # .word loop (placeholder)
        self.write_word(0x00000E814)  # lis $29
        self.write_word(0x00000000)  # .word bar (placeholder)
        self.write_word(0x8CA30000)  # lw $3, 0($29)
        self.write_word(0x00231820)  # add $3, $1, $3
        self.write_word(0xACA30000)  # sw $3, 0($29)
        self.write_word(0x00220820)  # add $1, $1, $2
        self.write_word(0x14200001)  # bne $1,$0, skip
        self.write_word(0x03E00008)  # jr $31
        self.write_word(0x03C00008)  # jr $28
        
        # Relocation records
        self.write_word(0x00000001)  # REL entry
        self.write_word(0x00000018)  # Address 0x18
        
        self.write_word(0x00000011)  # ESR entry
        self.write_word(0x00000020)  # Address 0x20
        self.write_word(0x00000003)  # Symbol len 3
        self.write_string("bar")  # Symbol "bar"
        
        self.write_word(0x00000005)  # ESD entry
        self.write_word(0x0000000C)  # Address 0x0C
        self.write_word(0x00000003)  # Symbol len 3
        self.write_string("foo")  # Symbol "foo"
        
        # End marker
        self.write_word(self.MERL_END)
        
        return bytes(self.data)
    
    def save_merl(self, filename: str, merl_data: bytes):
        """Save MERL data to file."""
        with open(filename, 'wb') as f:
            f.write(merl_data)
        print(f"Generated MERL file: {filename} ({len(merl_data)} bytes)")

def main():
    """Generate expected MERL files."""
    generator = MERLGenerator()
    
    # Generate m1.merl
    m1_data = generator.generate_m1_merl()
    generator.save_merl("asm_test/m1_expected.merl", m1_data)
    
    # Generate m2.merl
    m2_data = generator.generate_m2_merl()
    generator.save_merl("asm_test/m2_expected.merl", m2_data)
    
    print("\nGenerated expected MERL files:")
    print("- asm_test/m1_expected.merl")
    print("- asm_test/m2_expected.merl")
    print("\nUse merl_visualizer_expected.py to visualize these files.")

if __name__ == "__main__":
    main()
