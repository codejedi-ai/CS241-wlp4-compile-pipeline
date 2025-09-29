looking at the asm

Certainly! I will provide the assembly code for both `m1.asm` and `m2.asm` and then present the corresponding MERL (Module Entry Record Language) file contents in two separate Markdown tables.

Assembly Source Code


m1.asm

Code snippet

```
.import foo
.export bar

sw $31, -4($30)

lis $29
.word foo
jalr $29

lis $3
.word bar
lw $3, 0($3)

lw $31, -4($30)
jr $31

bar: .word 0

```


m2.asm

Code snippet

```
.export foo
.import bar

foo:
lis $2
.word -1
lis $28
.word loop
lis $29
.word bar

loop:
lw $3, 0($29)
add $3, $1, $3
sw $3, 0($29)
add $1, $1, $2
bne $1,$0, skip
jr $31

skip:
jr $28

```


MERL File Contents


MERL File for m1.asm

OffsetMERL CodeDescriptionAssembly Source0x000x10000002MERL cookie0x040x0000006CEnd of module0x080x00000034End of code0x0C0xAFDFFFC0MIPS program`sw $31, -4($30)`0x100x00000E814MIPS program`lis $29`0x140x00000000MIPS program`.word foo`0x180x03A00009MIPS program`jalr $29`0x1C0x000018814MIPS program`lis $3`0x200x00000030MIPS program`.word bar`0x240x8C630000MIPS program`lw $3, 0($3)`0x280x8FDFFFCMIPS program`lw $31, -4($30)`0x2C0x03E00008MIPS program`jr $31`0x300x00000000MIPS program`bar: .word 0`0x340x00000001REL entry0x380x00000020Address 0x200x3C0x00000011ESR entry0x400x00000014Address 0x140x440x00000003Symbol len 30x480x00000066f0x4C0x0000006Fo0x500x0000006Fo0x540x00000005ESD entry0x580x00000030Address 0x300x5C0x00000003Symbol len 30x600x00000062b0x640x00000061a0x680x00000072r

MERL File for m2.asm

OffsetMERL CodeDescriptionAssembly Source0x000x10000002MERL cookie0x040x00000078End of module0x080x00000040End of code0x0C0x000010814MIPS program`lis $2`0x100x0FFFFFFFMIPS program`.word -1`0x140x00000E814MIPS program`lis $28`0x180x00000024MIPS program`.word loop`0x1C0x00000E814MIPS program`lis $29`0x200x00000000MIPS program`.word bar`0x240x8CA30000MIPS program`lw $3, 0($29)`0x280x00231820MIPS program`add $3, $1, $3`0x2C0xACA30000MIPS program`sw $3, 0($29)`0x300x00220820MIPS program`add $1, $1, $2`0x340x14200001MIPS program`bne $1,$0, skip`0x380x03E00008MIPS program`jr $31`0x3C0x03C00008MIPS program`jr $28`0x400x00000001REL entry0x440x00000018Address 0x180x480x00000011ESR entry0x4C0x00000020Address 0x200x500x00000003Symbol len 30x540x00000062b0x580x00000061a0x5C0x00000072r0x600x00000005ESD entry0x640x0000000CAddress 0x0C0x680x00000003Symbol len 30x6C0x00000066f0x700x0000006Fo0x740x0000006Fo

What does .word do?

What does foo: do