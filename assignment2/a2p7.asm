stirling:
sw $1, -4($30)
sw $2, -8($30)
sw $4, -12($30)
sw $5, -16($30)
sw $6, -20($30)
sw $7, -24($30)
sw $31, -28($30)
lis $31
.word 28
sub $30, $30, $31
;f(0,0) = 1.
;f(i,0) = f(0,i) = 0, for integers i > 0.
;f(n,k) = (n-1) * f(n-1,k) + f(n-1,k-1), for integers n > 0 and k > 0.
; returns at $3


beq $1, $0, nis0
beq $0, $0, 3
nis0:
lis $3
.word 1
beq $2, $0, end


; Base case ; f(0,0) = 1.


add $3, $0, $0
beq $1, $0, end
beq $2, $0, end
; Base case ;f(i,0) = f(0,i) = 0, for integers i > 0.


lis $31
.word 1
sub $1, $1, $31 ; n = n - 1
sub $4, $1, $0 ; n = n - 1
lis $3
.word stirling
jalr $3
; have $4 be the temp register that stores (n - 1)
mult $4, $3
mflo $4 ; now $4 contains (n - 1) f(n - 1, k)
; Called f(n - 1, k)

lis $31
.word 1
sub $2, $2, $31 ; k = k - 1
lis $3
.word stirling
jalr $3
; have $4 be the temp register that stores (n - 1)
add $5, $3, $0 ; now $5 contains (n - 1) f(n - 1, k)
; Called f(n - 1, k - 1)
add $3, $4, $5 
end:
lis $31
.word 28
add $30, $30, $31
lw $1, -4($30)
lw $2, -8($30)
lw $4, -12($30)
lw $5, -16($30)
lw $6, -20($30)
lw $7, -24($30)
lw $31, -28($30)
jr $31


