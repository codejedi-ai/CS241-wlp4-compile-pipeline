sw $31, -4($30)

lis $29
.word foo
jalr $29

lis $3
.word foo
lw $3, 0($3)

lw $31, -4($30)
jr $31

foo: .word 0