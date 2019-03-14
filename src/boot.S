#include "arm/sysregs.h"
#include "mm.h"

//Make sure the code goes into the .text.boot section
.section ".text.boot"

.globl _start
_start:
	/*Move to ARM reg from coprocessor reg or special reg in this case
	multiprocessor affinity register */

	mrs	x0, mpidr_el1		//Read mpidr_el1 into x0

	//Description for mpdir_el1

	/*[7:0]	Aff0	Affinity level 0. Lowest level affinity field.
	Indicates the core number in the Cortex-A53 processor. The possible values are:
	0x0    A cluster with one core only.
	0x0, 0x1    A cluster with two cores.
	0x0, 0x1, 0x2    A cluster with three cores.
	0x0, 0x1, 0x2, 0x3    A cluster with four cores.*/

	and	x0, x0, #0xFF		//Check processor id

	//Compare and branch on zero so all cores which will not be used get trapped
	cbz	x0, master		  //And the one to be used will branch to master

	and x0, x0, #0xFE

	cbz x0, master1

	b	proc_hang

	//Infinite loop to trap the processors which we are not going to use (yet)
proc_hang:
	b 	proc_hang

	//About exception levels
	/*EL0 has the lowest software execution privilege, and execution at EL0 is
	called unprivileged execution.
	Increased values of n, indicate increased software execution privilege.
	EL2 provides support for processor virtualization.
	EL3 provides support for two security states
	Typically:
	EL0: Applications.
	EL1: OS kernel and associated functions that are typically described privileged.
	EL2: Hypervisor.
	EL3: Secure monitor.*/

	/*To change exception we can use SVC also hardware interrupts handlers
	when this happens there's a process
		Address of the current instruction is saved in the ELR_ELn register.
			Exception link register
	  Current processor state is stored in SPSR_ELn register
			Saved Program Status Register
	  An exception handler is executed (Remember microcontrollers)
	  Exception handler calls eret instruction. This instruction restores processor
		state from SPSR_ELn and resumes execution starting from the address,
		stored in the ELR_ELn register
	*/

	//The following will set the Exception level to EL1
master:
	//Set the values defined previously in the appropriate registers changing mode
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	//This step will set everything to change modes, flags and interrupts
	ldr	x0, =SPSR_VALUE_EL1
	msr	spsr_el3, x0

	//Address we return after eret instruction
	adr	x0, el1_entry
	msr	elr_el3, x0

	//This instruction changes modes
	eret

el1_entry:
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

	//After cleaning we set the stack pointer and pass execution to kernel_main
	mov	sp, #LOW_MEMORY
	bl	kernel_main
	b 	proc_hang		//Should never come here

master1:
	//Set the values defined previously in the appropriate registers changing mode
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el2, x0

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	//This step will set everything to change modes, flags and interrupts
	ldr	x0, =SPSR_VALUE_EL2
	msr	spsr_el3, x0

	//Address we return after eret instruction
	adr	x0, el2_entry
	msr	elr_el3, x0

	//This instruction changes modes
	eret

el2_entry:
	mov sp, #LOW_MEMORY1
	bl  kernel_main1
	b		proc_hang  //Should never come here
