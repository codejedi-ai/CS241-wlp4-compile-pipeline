add $3, $1, $0 ; Assuming $1 is larger
sltu $4, $3, $2 ; check $3 < $2
beq $4, $0, 1 ; if $1 > $2
add $3, $2, $0
jr $31