.globl memzero
memzero:
//XZR zero register for store instructions rather than move 0 to a known register
	STR xzr, [x0], #8
//x1 holds the size to be written with 0s so update until all have been done
	SUBS x1, x1, #8
//Loop if not finished
	B.GT memzero
	RET
