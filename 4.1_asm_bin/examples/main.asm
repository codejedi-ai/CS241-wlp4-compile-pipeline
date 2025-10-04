; Main program that imports functions from library
.export main
.import helper
.import printf

main:
    ; Call helper function (imported from library)
    .word 42
    .word 84
    jr $31
