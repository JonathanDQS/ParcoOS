//Remember xN is the same as wN but wN is a 32 bit form of the registers
.globl put32
put32:
	str w1,[x0] //Store data from the address specified in x0 into w1 (write)
	ret //Return instruction or inconditional branch if given a register
//Defaults to register x30 which is the link register

/*This piece of code will show the us the exception level we are in,
there are 4 of them*/

.globl get_el
get_el:
	mrs x0, CurrentEL //mrs instruction to get the value
	lsr x0, x0, #2		//and shift 2 places because those 2 bits are reserved and 0
ret

.globl get32 //Get data from the address specified in x0 into w0 (read)
get32:
	ldr w0,[x0]
	ret

.globl delay //Sets a delay of x0 cycles substracting until 0
delay:
	subs x0, x0, #1
	bne delay
	ret
