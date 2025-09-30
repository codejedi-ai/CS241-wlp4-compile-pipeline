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
sub $30 , $30, $4 ; update stack pointer
lis $3 ; set $3 to the value of 0
.word 0
sw $3, -8($29) ; push variable type: int name: rem
sub $30 , $30, $4 ; update stack pointer
lis $3 ; set $3 to the value of 0
.word 0
sw $3, -12($29) ; push variable type: int name: y
sub $30 , $30, $4 ; update stack pointer
lis $3 ; set $3 to the value of 0
.word 0
sw $3, -16($29) ; push variable type: int name: x
; statement lvalue BECOMES expr SEMI
lw $3, 0($29) ; which we will write as $3 <- a
sw $3, -16($29) ; which we will write as $3 -> $29 + -16 Written to x
; statement lvalue BECOMES expr SEMI
lw $3, -4($29) ; which we will write as $3 <- b
sw $3, -12($29) ; which we will write as $3 -> $29 + -12 Written to y
loop0:
; BEGIN TEST
lw $3, -12($29) ; which we will write as $3 <- y
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
lis $3
.word 0
add $30, $30, $4  ; 
lw $5,-4($30) ; -----------
slt $6, $3, $5 ; $6 = $3 < $5
slt $7, $5, $3 ; $7 = $5 < $3
add $3, $6, $7 ; Sum $6 and $7
; END TEST
beq $3, $0, endWhile0
; statement lvalue BECOMES expr SEMI
lw $3, -16($29) ; which we will write as $3 <- x
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
lw $3, -12($29) ; which we will write as $3 <- y
add $30, $30, $4  ; 
lw $5,-4($30) ; -----------
div $5, $3
mfhi $3
sw $3, -8($29) ; which we will write as $3 -> $29 + -8 Written to rem
; statement lvalue BECOMES expr SEMI
lw $3, -12($29) ; which we will write as $3 <- y
sw $3, -16($29) ; which we will write as $3 -> $29 + -16 Written to x
; statement lvalue BECOMES expr SEMI
lw $3, -8($29) ; which we will write as $3 <- rem
sw $3, -12($29) ; which we will write as $3 -> $29 + -12 Written to y
beq $0, $0, loop0
endWhile0:
; begin return
lw $3, -16($29) ; which we will write as $3 <- x
; begin epilogue
add $30 , $29, $4 ; update stack pointer
jr $31 
; end epilogue
