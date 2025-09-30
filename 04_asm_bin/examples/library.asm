; Library module that exports functions
.export helper
.export utility

helper:
    .word 100
    jr $31

utility:
    .word 200
    jr $31
