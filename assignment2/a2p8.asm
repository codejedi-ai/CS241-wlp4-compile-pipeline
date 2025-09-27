treeheight:
sw $4, -4($30) ; holds the parameter with is i it gets arr[i]
sw $5, -8($30) ; holds array address of the ith value
sw $6, -12($30) ; first child value
sw $7, -16($30) ; second child value
sw $8, -20($30) ; Buffer for the  
sw $9, -24($30) ; -1
sw $31, -28($30)
lis $31
.word 28
sub $30, $30, $31

; $3 returns the height of the array

lis $31
.word 4
mult $4, $31
mflo $31 ; $31 = i * 4
add $5, $1, $31 ; Obtained child node pointer
lis $31 
.word 4
add $5, $5, $31 ; first child pointer
add $6, $5, $31  ; Second child pointer
lw $5, 0($5) ; first child value
lw $6, 0($6) ; second child value
; If both $5 and $6 equals to negative 1



lis $8
.word 1

sub $9, $0, $8
; Now $9 is negative one

lis $8
.word 0
lis $3
.word 0
beq $5, $9, secondNodeCheck
; -----------------------------
lis $31
.word 1

add $4, $5, $0
lis $3 
.word treeheight
jalr $3

add $8, $0, $3
secondNodeCheck: beq $6, $9, end
; -----------------------------
lis $31
.word 1

add $4, $6, $0
lis $3 
.word treeheight
jalr $3



end:
;--------------------
slt $31, $1, $2 ; check $3 < $8
beq $31, $0, 1 ; if $3 > $8
add $3, $8, $0
;--------------------


lis $31
.word 1
add $3, $3, $31





lis $31
.word 28
add $30, $30, $31
lw $4, -4($30) ; holds array address
lw $5, -8($30)
lw $6, -12($30)
lw $7, -16($30)
lw $8, -20($30)
lw $9, -24($30)
lw $31, -28($30)
jr $31


