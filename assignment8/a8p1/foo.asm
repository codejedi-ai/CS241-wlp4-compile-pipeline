.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $11
.word 1
beq $0, $0, wain
; procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
fact: ; begin prologue ======== ANOUTHER FUNCTION: fact
sub $29, $30, $4 ; setup frame pointer
; end prologue
sub $30 , $30, $4 ; update stack pointer
lis $3 ; set $3 to the value of 0
.word 0
sw $3, 0($29) ; push variable type: int name: ret
lw $3, 4($29) ; which we will write as $3 <- n
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
lis $3
.word 0
add $30, $30, $4  ; 
lw $5,-4($30) ; -----------
slt $6, $3, $5 ; $6 = $3 < $5
slt $7, $5, $3 ; $7 = $5 < $3
add $3, $6, $7 ; Sum $6 and $7
sub $3, $11, $3 ; negation
beq $3, $0, else0
; statement lvalue BECOMES expr SEMI
lis $3
.word 1
sw $3, 0($29) ; which we will write as $3 -> $29 + 0 Written to ret
beq $0, $0, endif0
else0:
; statement lvalue BECOMES expr SEMI
lw $3, 4($29) ; which we will write as $3 <- n
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
; Calling anouther function: fact =============== 
sw $29, -4($30) ; push the value of $29 onto the stack
sub $30, $30, $4 ; -----------
sw $31, -4($30) ; push the value of $31 onto the stack
sub $30, $30, $4 ; -----------
sw $28, -4($30) ; push the value of $28 onto the stack
sub $30, $30, $4 ; -----------
add $28, $30, $0
; ARgument Number# 0
lw $3, 4($29) ; which we will write as $3 <- n
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
; start term children expr expr MINUS term
lis $3
.word 1
; end term children
add $30, $30, $4  ; 
lw $5,-4($30) ; -----------
; =========== int - int
sub $3, $5, $3
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
lis $5
.word fact
jalr $5
add $30, $28, $0 ; this is the restore the stack
add $30, $30, $4  ; 
lw $28,-4($30) ; -----------
add $30, $30, $4  ; 
lw $31,-4($30) ; -----------
add $30, $30, $4  ; 
lw $29,-4($30) ; -----------
add $30, $30, $4  ; 
lw $5,-4($30) ; -----------
mult $5, $3
mflo $3
sw $3, 0($29) ; which we will write as $3 -> $29 + 0 Written to ret
endif0:
; begin return
lw $3, 0($29) ; which we will write as $3 <- ret
; begin epilogue
add $30 , $29, $4 ; update stack pointer
jr $31 
; end epilogue
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
; Calling anouther function: fact =============== 
sw $29, -4($30) ; push the value of $29 onto the stack
sub $30, $30, $4 ; -----------
sw $31, -4($30) ; push the value of $31 onto the stack
sub $30, $30, $4 ; -----------
sw $28, -4($30) ; push the value of $28 onto the stack
sub $30, $30, $4 ; -----------
add $28, $30, $0
; ARgument Number# 0
lis $3
.word 5
sw $3, -4($30) ; push the value of $3 onto the stack
sub $30, $30, $4 ; -----------
lis $5
.word fact
jalr $5
add $30, $28, $0 ; this is the restore the stack
add $30, $30, $4  ; 
lw $28,-4($30) ; -----------
add $30, $30, $4  ; 
lw $31,-4($30) ; -----------
add $30, $30, $4  ; 
lw $29,-4($30) ; -----------
; begin epilogue
add $30 , $29, $4 ; update stack pointer
jr $31 
; end epilogue
