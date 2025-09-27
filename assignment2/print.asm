; Wrapper function
print:
sw $2, -4($30)
sw $20, -8($30)
sw $31, -12($30)
sw $1, -16($30)
lis $31
.word 16
sub $30, $30, $31
;Made stack
;temp registers
;$2 the print register
;$20 the print address register
; 48 is '0' in ascii

lis $2
.word 48
beq $1, $0, endWrapper ; if $1 == 0

slt $2, $1, $0 ; if $1 is negative then $2 == 1

beq $2, $0, endNegative ; if $2 == 0
; code for the case if it is negative

lis $2
.word 1
sub $2, $0, $2
mult $1, $2
; inverted the value $1
mflo $1
lis $2
.word 45
lis $20 ; output memory sw $s, 0($20) to print the value in $s as ascii
.word 0xFFFF000c
sw $2, 0($20)
endNegative:
;FInally the recursive case where $1

lis $20
.word printBase10Positive
jalr $20
lis $20 ; output memory sw $s, 0($20) to print the value in $s as ascii
.word 0xFFFF000c
lis $2
.word 10

sw $2, 0($20)
endWrapper:
lis $31
.word 16
add $30, $30, $31
lw $2, -4($30)
lw $20, -8($30)
lw $31, -12($30)
lw $1, -16($30)
jr $31


printBase10Positive:
; prints the base 10 number NUMBER MUST BE POSITIVE. No New line
; $1 is the number in need of printing 
sw $2, -4($30)
sw $20, -8($30)
sw $31, -12($30)
lis $31
.word 12
sub $30, $30, $31
; Made stack
; Base case
beq $1, $0, end

lis $2
.word 10
div $1, $2
mfhi $2 ;remainder $2 is SEALED no changing it except for printing 
mflo $1 ; quotient pass $1 into the next layer of recursion
lis $20
.word printBase10Positive

jalr $20
lis $20
.word 48
add $2, $2, $20
lis $20 ; output memory sw $s, 0($20) to print the value in $s as ascii
.word 0xFFFF000c
sw $2, 0($20)


end:
lis $31
.word 12
add $30, $30, $31
lw $2, -4($30)
lw $20, -8($30)
lw $31, -12($30)
jr $31