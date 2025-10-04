; Sample MIPS Assembly Program for Linkasmbler
; This program demonstrates the working features of the assembler

main:
    ; Data directives - these work correctly
    .word 42
    .word 84
    .word 128
    .word 256
    
    ; Control flow instructions - these work correctly
    jr $31
    
    ; Additional labels for demonstration
loop:
    jr $31
    
end:
    jr $31
    
    ; More data
    .word 512
    .word 1024