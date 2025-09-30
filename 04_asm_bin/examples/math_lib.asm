; Math Library - Exports mathematical functions
.export addnumbers
.export multiplynumbers
.export getconstant

addnumbers:
    ; Function that adds two numbers
    .word 100
    .word 200
    jr $31

multiplynumbers:
    ; Function that multiplies two numbers  
    .word 300
    .word 400
    jr $31

getconstant:
    ; Function that returns a constant value
    .word 42
    jr $31