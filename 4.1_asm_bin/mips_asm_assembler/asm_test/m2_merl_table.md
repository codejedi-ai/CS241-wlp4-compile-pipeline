# MERL File Analysis: m2.merl

## File Information

- **Format**: Simplified
- **Code Size**: 60 bytes
- **Total Size**: 57 bytes
- **Has Relocation Records**: False

## MERL File Contents

| Offset | Hex Value | Type | Description | Assembly |
|--------|-----------|------|-------------|----------|
| 0x00000000 | 0x10000002 | Header | MERL Magic Number |  |
| 0x00000004 | 0x0000003C | Header | Code Size |  |
| 0x00000008 | 0x00003C40 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x0000000C | 0x0000FF3F | Code | MIPS Instruction | unknown_r_type (funct=63) |
| 0x00000010 | 0x80000000 | Code | MIPS Instruction | unknown_i_type (opcode=32) |
| 0x00000014 | 0x3FA00000 | Code | MIPS Instruction | lis $0 |
| 0x00000018 | 0x008FA300 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x0000001C | 0x00002318 | Code | MIPS Instruction | mult $0, $0 |
| 0x00000020 | 0x20AFA300 | Code | MIPS Instruction | unknown_i_type (opcode=8) |
| 0x00000024 | 0x00002208 | Code | MIPS Instruction | jr $0 |
| 0x00000028 | 0x20142000 | Code | MIPS Instruction | unknown_i_type (opcode=8) |
| 0x0000002C | 0x0003E000 | Code | MIPS Instruction | unknown_r_type (funct=0) |
| 0x00000030 | 0x08038000 | Code | MIPS Instruction | unknown_i_type (opcode=2) |
| 0x00000034 | 0x08100000 | Code | MIPS Instruction | unknown_i_type (opcode=2) |
| 0x00000038 | <incomplete> | Padding | Remaining bytes | |
