; use iteration to find the maximum value of the array

lw $5, 0($1) ; $5 holds the first element
add $3, $5, $0 ; base case load the first element of the array into $3
add $6, $1, $0 ;
lis $7
.word 1 
lis $8
.word 4 


loop:

lw $5, 0($6) ; $5 holds the first element

sltu $9, $3, $5 ; check $3 < $5
beq $9, $0, 1 ; if $3 > $5
add $3, $5, $0

add $6, $6, $8 ; $6 holds the array address next memory address
add $4, $4, $7 ; i++

bne $4, $2, loop

jr $31