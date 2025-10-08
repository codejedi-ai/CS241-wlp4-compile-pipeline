.import init
.import print
.export wain
.export helper
lis $4
.word 4
wain:
add $1, $2, $3
helper:
jr $31
