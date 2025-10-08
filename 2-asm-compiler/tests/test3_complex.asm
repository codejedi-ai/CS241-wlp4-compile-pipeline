.import init
.export wain
lis $4
.word 4
beq $0, $0, wain
add $1, $2, $3
wain:
jr $31
