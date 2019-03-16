#ifndef	_P_MINI_UART_H
#define	_P_MINI_UART_H

//Use base and offsets
#include "peripherals/base.h"

//Base: start of memory reserved for devices by Pi 3, BCM2835 manual page 8
#define AUX_ENABLES     		(PBASE+0x00215004)
//Mini UART I/O data
#define AUX_MU_IO_REG   		(PBASE+0x00215040)
//Mini UART Interrupt Enable - enables interrupts
#define AUX_MU_IER_REG  		(PBASE+0x00215044)
//Mini UART Interrupt Identify - show interupt status
#define AUX_MU_IIR_REG  		(PBASE+0x00215048)
//Mini UART Line Control
#define AUX_MU_LCR_REG  		(PBASE+0x0021504C)
//Mini UART Modem Control
#define AUX_MU_MCR_REG  		(PBASE+0x00215050)
//Mini UART Line Status
#define AUX_MU_LSR_REG  		(PBASE+0x00215054)
//Mini UART Modem Status
#define AUX_MU_MSR_REG  		(PBASE+0x00215058)
//Mini UART Scratch
#define AUX_MU_SCRATCH  		(PBASE+0x0021505C)
//Mini UART Extra Control
#define AUX_MU_CNTL_REG 		(PBASE+0x00215060)
//Mini UART Extra Status
#define AUX_MU_STAT_REG 		(PBASE+0x00215064)
//Mini UART Baudrate
#define AUX_MU_BAUD_REG 		(PBASE+0x00215068)

//MHz
#define MHz 1000000

//System clock frequency
#define SYS_CLOCK_FREQ 			(250 * MHz)

//For user input we use Mini UART Interrupt Enable
//Usage of this register in BCM2835, manual page 12
//If bit 0 set, interrupt asserted when FIFO has at least 1 byte
#define IER_REG_EN_REC_INT 	(1 << 0)
//If bit 1 set, interrupt asserted when FIFO is empty
#define IER_REG_EN_EMP_INT 	(0 << 1)
//Must be set to receive interrupts
#define IER_REG_INT        	(3 << 2)
#define IER_REG_VALUE      	(IER_REG_EN_REC_INT|IER_REG_EN_EMP_INT|IER_REG_INT)

/*Regarding AUX_MU_IIR_REG we have:
Bits 2:1
On read: shows interupt ID bit
	00: No interupt,
	01: Transmit holding register empty
	10: Receiver holds valid bit
  11: Not possible
On write:
  Bit 1 set clears the receive FIFO
  Bit 2 set clears the transmit FIFO*/
#define IIR_REG_REC_NON_EMPTY 	(1 << 2)
#define CLEAR_FIFO              (3 << 1)

#endif  /*_P_MINI_UART_H */
