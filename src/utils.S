//Remember xN is the same as wN but wN is a 32 bit form of the registers
.globl put32
put32:
	str w1,[x0] //Store data from the address specified in x0 into w1 (write)
	ret //Return instruction or inconditional branch if given a register
//Defaults to register x30 which is the link register

.globl get32 //Get data from the address specified in x0 into w0 (read)
get32:
	ldr w0,[x0]
	ret

.globl delay //Sets a delay of x0 cycles substracting until 0
delay:
	subs x0, x0, #1
	bne delay
	ret

.globl put321
put321:
	add	w1, w1, #1
	str w1,[x0] //Store data from the address specified in x0 into w1 (write)
	ret
