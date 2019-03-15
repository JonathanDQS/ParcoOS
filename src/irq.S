//Here we set the address of the vectors, handlers in the appropriate register
.globl irq_vector_init
irq_vector_init:
	adr	x0, vectors			//Load VBAR_EL1 with table address
	msr	vbar_el1, x0
	ret

//Enable normal interrupts
.globl enable_irq
enable_irq:
	msr daifclr, #2
	ret

//Disable normal interrupts
.globl disable_irq
disable_irq:
	msr	daifset, #2
	ret

/*We do this because the 4 bits which control interrupts are
3  D debug exceptions. These are a special type of synchronous exceptions.
	 It is not possible to mask all synchronous exceptions, but it is convenient
	 to have a separate flag that can mask debug exceptions.
2  A SErrors. It is called A because SErrors sometimes are called
	 asynchronous aborts.
1  I IRQs
0  F FIQs*/
