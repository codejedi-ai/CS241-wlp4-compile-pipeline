.import print ; Import print function for output
.import init ; Import init function for memory management
.import new ; Import new function for dynamic allocation
.import delete ; Import delete function for memory deallocation
.import putchar ; Import putchar function for character output
.import getchar ; Import getchar function for character input
lis $4 ; Load word size constant
.word 4 ; Store word size (4 bytes) in $4
lis $11 ; Load constant 1
.word 1 ; Store 1 in $11 (used for comparisons and NULL)
beq $0, $0, wain ; Jump to main function (unconditional branch)
wain: ; Main function entry point
; === FUNCTION PROLOGUE ===
sub $29, $30, $4 ; Set up frame pointer ($29) to point to first local variable
sub $30, $30, $4 ; Allocate space for first parameter on stack
sw $1, 0($29) ; Store first parameter (a) from $1 to stack frame
sub $30, $30, $4 ; Allocate space for second parameter on stack
sw $2, -4($29) ; Store second parameter (b) from $2 to stack frame
; === END PROLOGUE ===
;  ================ Pushing $2 to stack ================ 
sw $2, -4($30) ; Store $2 at top of stack
sub $30, $30, $4 ; Decrement stack pointer by 4 bytes
add $2, $0, $0 ; Set $2 to 0 for int parameter (no array initialization needed)
;  ================ Pushing $31 to stack ================ 
sw $31, -4($30) ; Store $31 at top of stack
sub $30, $30, $4 ; Decrement stack pointer by 4 bytes
lis $5 ; Load address of init function
.word init ; Init function label
jalr $5 ; Call init function for memory management
;  ================ Popping $31 from stack ================ 
add $30, $30, $4 ; Increment stack pointer by 4 bytes
lw $31,-4($30) ; Load $31 from top of stack
;  ================ Popping $2 from stack ================ 
add $30, $30, $4 ; Increment stack pointer by 4 bytes
lw $2,-4($30) ; Load $2 from top of stack
; === FUNCTION EPILOGUE ===
; Evaluate return expression
; Addition operation: expr expr PLUS term
lw $3, 0($29) ; Load variable 'a' from stack frame into $3
;  ================ Pushing $3 to stack ================ 
sw $3, -4($30) ; Store $3 at top of stack
sub $30, $30, $4 ; Decrement stack pointer by 4 bytes
lw $3, -4($29) ; Load variable 'b' from stack frame into $3
;  ================ Popping $5 from stack ================ 
add $30, $30, $4 ; Increment stack pointer by 4 bytes
lw $5,-4($30) ; Load $5 from top of stack
; Adding int + int
add $3, $5, $3 ; Perform addition: $3 = $5 + $3
; Restore stack frame and return
add $30, $29, $4 ; Restore stack pointer to original position
jr $31 ; Return to caller
; === END EPILOGUE ===

