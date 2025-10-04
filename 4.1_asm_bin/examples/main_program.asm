; Main Program - Imports functions from math library
.export main
.import addnumbers
.import multiplynumbers
.import getconstant

main:
    ; Main program that uses imported functions
    .word 10
    .word 20
    .word 30
    jr $31