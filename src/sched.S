#include "sched.h"

//Switch context of process
.globl cpu_switch_to
cpu_switch_to:
	MOV	x10, #THREAD_CPU_CONTEXT	//Offset to cpu_context structure
	ADD	x8, x0, x10								//x0 contains pointer to first argument
																//(task we are switching from)
																//Now x8 has pointer to current cpu_context
	MOV	x9, sp										//Save stack pointer into x9
	STP	x19, x20, [x8], #16				//Store current task registers
	STP	x21, x22, [x8], #16
	STP	x23, x24, [x8], #16
	STP	x25, x26, [x8], #16
	STP	x27, x28, [x8], #16
	STP	x29, x9, [x8], #16
	STR	x30, [x8]									//Including link register of course
	ADD	x8, x1, x10								//x1 contains next task pointer so we move to
																//that cpu_context and restore it for it to be
	LDP	x19, x20, [x8], #16				//ready to run
	LDP	x21, x22, [x8], #16
	LDP	x23, x24, [x8], #16
	LDP	x25, x26, [x8], #16
	LDP	x27, x28, [x8], #16
	LDP	x29, x9, [x8], #16
	LDR	x30, [x8]									//Process to go to
	MOV	sp, x9										//Restore stack pointer
	RET														//Return, go to the address in the link register
