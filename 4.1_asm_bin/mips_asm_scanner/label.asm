; Simple MIPS assembly showing .word LABEL usage
; foo function adds $1 and $2, stores result in $3

; Main program
lis $5
.word foo           ; Load address of foo function
jalr $5             ; Call foo function

jr $31              ; Exit

; Addition function
foo:
    add $3, $1, $2  ; $3 = $1 + $2
    jr $31          ; Return