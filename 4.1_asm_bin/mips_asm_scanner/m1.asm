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