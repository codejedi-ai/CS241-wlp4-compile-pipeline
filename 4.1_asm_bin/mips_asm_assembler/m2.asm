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