.export print
.export init
.export new
.export delete
.export putchar
.export getchar

print:
	; expects: $1 = int to print
	; stub: do nothing
	jr $31

init:
	; stub: do nothing
	jr $31

new:
	; expects: $1 = number of elements
	; return: $3 = pointer (0 on failure)
	; stub: always fail/NULL
	add $3, $0, $0
	jr $31

delete:
	; expects: $1 = pointer to free
	; stub: do nothing
	jr $31

putchar:
	; expects: $1 = char code
	; stub: do nothing
	jr $31

getchar:
	; return: $3 = char code or -1 on EOF
	; stub: always return 0
	add $3, $0, $0
	jr $31