.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $11
.word 1
beq $0, $0, wain
wain: ; begin prologue
sub $29, $30, $4 ; setup frame pointer
sub $30 , $30, $4 ; update stack pointer
sw $1, 0($29) ; push variable a
sub $30 , $30, $4 ; update stack pointer
sw $2, -4($29) ; push variable b
; end prologue
sw $2, -4($30) ; push the value of $2 onto the stack
sub $30, $30, $4 ; -----------
add $2, $0, $0
sw $31, -4($30) ; push the value of $31 onto the stack
sub $30, $30, $4 ; -----------
lis $5
.word init
jalr $5
add $30, $30, $4  ; 
lw $31,-4($30) ; -----------
add $30, $30, $4  ; 
lw $2,-4($30) ; -----------
; begin return
lis $3
.word 42
; begin epilogue
add $30 , $29, $4 ; update stack pointer
jr $31 
; end epilogue
