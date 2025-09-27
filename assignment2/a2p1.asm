Max: ; parameters $1 $2 and outputs on $3
sw $1, -4($30) ; holds the parameter with is i it gets arr[i]
sw $2, -8($30) ; holds array address of the ith value
sw $4, -12($30) ; second child value
sw $31, -16($30)
lis $31
.word 20
sub $30, $30, $31


add $3, $1, $0 ; Assuming $1 is larger
slt $4, $1, $2 ; check $1 < $2
beq $4, $0, 1 ; if $1 > $2
add $3, $2, $0


lis $31
.word 20
add $30, $30, $31
lw $1, -4($30) ; holds the parameter with is i it gets arr[i]
lw $2, -8($30) ; holds array address of the ith value
lw $4, -12($30) ; second child value
lw $31, -16($30)
jr $31
