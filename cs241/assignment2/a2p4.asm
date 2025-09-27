; use iteration to find the maximum value of the array
; $3 is the output value
add $6, $1, $0 ;
lis $7
.word 1
lis $8
.word 4 
lis $9
.word 64
lis $20 ; output memory
.word 0xFFFF000c

loop:

lw $5, 0($6) ; $5 holds the current element

lis $3
.word 32

beq $5, $0, print
add $3, $5, $9



print:
sw $3, 0($20)
add $6, $6, $8 ; $6 holds the array address next memory address
add $4, $4, $7 ; i++
bne $4, $2, loop
jr $31






