/*There's a difference between exceptions and interrupts:
Exceptions: triggered in the execution flow, for example by an SVC instruction
Interrupts and fast interrupts: asynchronous and triggered by external hardware
Fast interrupts have higher priority than normal interrupts
In ARMv8 we also have SError or system error: indicate some error condition.
*/

/*We need handlers for interrupts, in the form of adresses where we "branch"
once an interrupt occurs, these depend on state and interrupt
we have 4 states regardign interrupt handling:
  EL1t: Exception is taken from EL1 while stack pointer was shared with EL0.
    This happens when SPSel register holds the value 0.
  EL1h: Exception is taken from EL1 at the time when dedicated stack pointer
    was allocated for EL1. This means that SPSel holds the value 1 and this
    is the mode that we are currently using.
  EL0_64: Exception is taken from EL0 executing in 64-bit mode.
  EL0_32: Exception is taken from EL0 executing in 32-bit mode.

So 4 exception levels: Exceptions, IRQ, FIQ, and SError
and 4 states EL1t, EL1h, EL0_64, EL0_32 we need 16 exception handlers
Offset for exception table in ARM manual page 1876.
Usually not much espace to work within the space given in the table so use it
to jump to the handler code.

Handler: sequence of instructions to be executed when an exception occurs.
*/

#include "entry.h"

//Handler for not implemented interrupts which should not raise
	.macro handle_invalid_entry type
	kernel_entry
	MOV	x0, #\type                  //Pass paremeters to function - handler used
	MRS	x1, esr_el1                 //Exception Syndrome Register - page 2431
	MRS	x2, elr_el1                 /*Contains the address of the instruction
 that had been executed when the exception was generated*/
	BL	show_invalid_entry_message  //Branch to message printed
	B	  err_hang
	.endm

  //We use this macro to create entries in the table
	.macro	ventry	label
	.align	7
   /*Use it because it aligns in a position 2^7 which is the space given is
   0x80 and it is 128 bytes*/
	B	\label
	.endm

  /*This saves the state - the whole 30 general purpose registers - not really
  necessary but convenient for now*/
/*	.macro	kernel_entry
	SUB	sp, sp, #S_FRAME_SIZE
  //STP store pair of registers
	STP	x0, x1, [sp], #16
	STP	x2, x3, [sp], #16
	STP	x4, x5, [sp], #16
	STP	x6, x7, [sp], #16
	STP	x8, x9, [sp], #16
	STP	x10, x11, [sp], #16
	STP	x12, x13, [sp], #16
	STP	x14, x15, [sp], #16
	STP	x16, x17, [sp], #16
	STP	x18, x19, [sp], #16
	STP	x20, x21, [sp], #16
	STP	x22, x23, [sp], #16
	STP	x24, x25, [sp], #16
	STP	x26, x27, [sp], #16
	STP	x28, x29, [sp], #16
	STR	x30, [sp]
	.endm
*/
	.macro	kernel_entry
	SUB	sp, sp, #S_FRAME_SIZE
	STP	x0, x1, [sp, #16 * 0]
	STP	x2, x3, [sp, #16 * 1]
	STP	x4, x5, [sp, #16 * 2]
	STP	x6, x7, [sp, #16 * 3]
	STP	x8, x9, [sp, #16 * 4]
	STP	x10, x11, [sp, #16 * 5]
	STP	x12, x13, [sp, #16 * 6]
	STP	x14, x15, [sp, #16 * 7]
	STP	x16, x17, [sp, #16 * 8]
	STP	x18, x19, [sp, #16 * 9]
	STP	x20, x21, [sp, #16 * 10]
	STP	x22, x23, [sp, #16 * 11]
	STP	x24, x25, [sp, #16 * 12]
	STP	x26, x27, [sp, #16 * 13]
	STP	x28, x29, [sp, #16 * 14]
	STR	x30, [sp, #16 * 15]
	.endm

  //This returns the values to the registers
	.macro	kernel_exit
  //Load pair of registers
	LDP	x0, x1, [sp, #16 * 0]
	LDP	x2, x3, [sp, #16 * 1]
	LDP	x4, x5, [sp, #16 * 2]
	LDP	x6, x7, [sp, #16 * 3]
	LDP	x8, x9, [sp, #16 * 4]
	LDP	x10, x11, [sp, #16 * 5]
	LDP	x12, x13, [sp, #16 * 6]
	LDP	x14, x15, [sp, #16 * 7]
	LDP	x16, x17, [sp, #16 * 8]
	LDP	x18, x19, [sp, #16 * 9]
	LDP	x20, x21, [sp, #16 * 10]
	LDP	x22, x23, [sp, #16 * 11]
	LDP	x24, x25, [sp, #16 * 12]
	LDP	x26, x27, [sp, #16 * 13]
	LDP	x28, x29, [sp, #16 * 14]
	LDR	x30, [sp, #16 * 15]
	ADD	sp, sp, #S_FRAME_SIZE
	ERET
	.endm


/*Exception vectors definitions - for now IRQ implemented and the others go to
a single handler just in case but we need to define all*/
.align  11
.globl  vectors
vectors:
	ventry	sync_invalid_el1t			// Synchronous EL1t
	ventry	irq_invalid_el1t			// IRQ EL1t
	ventry	fiq_invalid_el1t			// FIQ EL1t
	ventry	error_invalid_el1t		// Error EL1t

	ventry	sync_invalid_el1h			// Synchronous EL1h
	ventry	el1_irq								// IRQ EL1h
	ventry	fiq_invalid_el1h			// FIQ EL1h
	ventry	error_invalid_el1h		// Error EL1h

	ventry	sync_invalid_el0_64			// Synchronous 64-bit EL0
	ventry	irq_invalid_el0_64			// IRQ 64-bit EL0
	ventry	fiq_invalid_el0_64			// FIQ 64-bit EL0
	ventry	error_invalid_el0_64		// Error 64-bit EL0

	ventry	sync_invalid_el0_32			// Synchronous 32-bit EL0
	ventry	irq_invalid_el0_32			// IRQ 32-bit EL0
	ventry	fiq_invalid_el0_32			// FIQ 32-bit EL0
	ventry	error_invalid_el0_32		// Error 32-bit EL0

sync_invalid_el1t:
	handle_invalid_entry  SYNC_INVALID_EL1t

irq_invalid_el1t:
	handle_invalid_entry  IRQ_INVALID_EL1t

fiq_invalid_el1t:
	handle_invalid_entry  FIQ_INVALID_EL1t

error_invalid_el1t:
	handle_invalid_entry  ERROR_INVALID_EL1t

sync_invalid_el1h:
	handle_invalid_entry  SYNC_INVALID_EL1h

fiq_invalid_el1h:
	handle_invalid_entry  FIQ_INVALID_EL1h

error_invalid_el1h:
	handle_invalid_entry  ERROR_INVALID_EL1h

sync_invalid_el0_64:
	handle_invalid_entry  SYNC_INVALID_EL0_64

irq_invalid_el0_64:
	handle_invalid_entry  IRQ_INVALID_EL0_64

fiq_invalid_el0_64:
	handle_invalid_entry  FIQ_INVALID_EL0_64

error_invalid_el0_64:
	handle_invalid_entry  ERROR_INVALID_EL0_64

sync_invalid_el0_32:
	handle_invalid_entry  SYNC_INVALID_EL0_32

irq_invalid_el0_32:
	handle_invalid_entry  IRQ_INVALID_EL0_32

fiq_invalid_el0_32:
	handle_invalid_entry  FIQ_INVALID_EL0_32

error_invalid_el0_32:
	handle_invalid_entry  ERROR_INVALID_EL0_32

el1_irq:
	kernel_entry
	BL	handle_irq
	kernel_exit

//Go to sleep if we did not handle it
.globl err_hang
err_hang: B err_hang
