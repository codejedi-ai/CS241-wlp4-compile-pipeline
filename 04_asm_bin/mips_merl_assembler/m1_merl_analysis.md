# MERL File Analysis: m1.merl

## File Information

- **Format**: Simplified
- **Code Size**: 60 bytes
- **Total Size**: 72 bytes
- **Has Relocation Records**: False

## MERL File Contents

| Offset | Hex Value | Type | Description | Assembly |
|--------|-----------|------|-------------|----------|
| 0x00000000 | 0x10000002 | Header | MERL Magic Number |  |
| 0x00000004 | 0x00000048 | Header | Code Size |  |
| 0x00000008 | 0x00000034 | Header | End of Code |  |
| 0x0000000C | 0xAFDFFFFC | Code | MIPS Instruction | sw $31, -4($30) |
| 0x00000010 | 0x0000E814 | Code | MIPS Instruction | unknown_r_type (funct=20) |
| 0x00000014 | 0x00000000 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x00000018 | 0x03A00009 | Code | MIPS Instruction | jalr $29 |
| 0x0000001C | 0x00001814 | Code | MIPS Instruction | unknown_r_type (funct=20) |
| 0x00000020 | 0x00000024 | Code | MIPS Instruction | unknown_r_type (funct=36) |
| 0x00000024 | 0x8C630000 | Code | MIPS Instruction | lw $3, 0($3) |
| 0x00000028 | 0x8FDFFFFC | Code | MIPS Instruction | lw $31, -4($30) |
| 0x0000002C | 0x03E00008 | Code | MIPS Instruction | jr $31 |
| 0x00000030 | 0x00000000 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x00000034 | 0x00000008 | Code | MIPS Instruction | jr $0 |
| 0x00000038 | 0x00000001 | Code | MIPS Instruction | unknown_r_type (funct=1) |
| 0x0000003C | 0x00000014 | Code | MIPS Instruction | unknown_r_type (funct=20) |
| 0x00000040 | 0x00000000 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x00000044 | 0x10000001 | Footer | MERL End Marker |  |
