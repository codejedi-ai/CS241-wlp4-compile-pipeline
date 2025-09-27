lis $3
.word 1
lis $4 ; initialize $4 = 1
.word 1
lis $5 ; initialize $5 = 4
.word 4
lis $3 ; initialize $3 = 1
.word 1
add $6, $0, $0 ; initialize i = 0
add $10, $1, $0 ; set the value of $10 to be the value of the first index in the array
sub $12, $2, $4
loop1:
lw $7, 0($10)
lw $8, 4($10)
slt $11, $8, $7 ; $7 > $8
beq $11, $0, else1
add $3, $0, $0
jr $31
else1:
add $6, $6, $4
add $10, $10, $5
bne $6, $12, loop1
jr $31