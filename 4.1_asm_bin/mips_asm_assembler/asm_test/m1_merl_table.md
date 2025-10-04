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
| 0x0000000C | 0xFFFC3FA0 | Code | MIPS Instruction | unknown_i_type (opcode=63) |
| 0x00000010 | 0x00000003 | Code | MIPS Instruction | unknown_r_type (funct=3) |
| 0x00000014 | 0xA000093C | Code | MIPS Instruction | unknown_i_type (opcode=40) |
| 0x00000018 | 0x60000000 | Code | MIPS Instruction | unknown_i_type (opcode=24) |
| 0x0000001C | 0x8C630000 | Code | MIPS Instruction | lw $3, 0($3) |
| 0x00000020 | 0x8FDFFFFC | Code | MIPS Instruction | lw $31, -4($30) |
| 0x00000024 | 0x03E00008 | Code | MIPS Instruction | jr $31 |
| 0x00000028 | 0x00100000 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x0000002C | <incomplete> | Padding | Remaining bytes | |
