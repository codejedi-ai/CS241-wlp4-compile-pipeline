; Test file with import/export functionality
.export main
.import printf

main:
    .word 42
    .word 84
    jr $31
