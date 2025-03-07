//Constants and others to be used
#include "utils.h"
#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"

//To use UART we need to activate GPIO pins 14 and 15
void uart_init (unsigned int baudrate)
{
	unsigned int selector;

	//With a variable baudrate
	unsigned int baudrate_reg;
	if (baudrate > 0 && baudrate < 115200)
	{
	 	baudrate_reg = (unsigned int)((SYS_CLOCK_FREQ >> 3)/(baudrate + 1));
	}
	else	baudrate_reg = 270;

	baudrate_reg &= 0x0000FFFF; //Since the register only takes the 16 lower bits

	//Get register which controls function of GPIOs from 10 to 19
	selector = get32(GPFSEL1);
	/*Bits 14:12 of the GPFSEL1 choose the function of the GPIO pin 14
	000 = GPIO Pin n is an input
	001 = GPIO Pin n is an output
	100 = GPIO Pin n takes alternate function 0
	101 = GPIO Pin n takes alternate function 1
	110 = GPIO Pin n takes alternate function 2
	111 = GPIO Pin n takes alternate function 3
	011 = GPIO Pin n takes alternate function 4
	010 = GPIO Pin n takes alternate function 5*/
	//Refer to table to check alternate function available for GPIO - page 102
	selector &= ~(7<<12);                   //Clean GPIO14 function bits
	selector |= 2<<12;                      //Set alt5 for GPIO14 which is TXD1
	//TXD1 serial communication (for mini_uart in our case)
	/*Bits 17:15 of the GPFSEL1 choose the function of the GPIO pin 15*/
	selector &= ~(7<<15);                   //Clean GPIO15 function bits
	selector |= 2<<15;                      //Set alt5 for GPIO15 which is RXD1
	//RXD1 serial communication (for mini_uart in our case)
	//Write the changes to the appropiate address
	put32(GPFSEL1,selector);

	/*00 = Off – disable pull-up/down
	01 = Enable Pull Down control
	10 = Enable Pull Up control
	11 = Reserved
	*Use in conjunction with GPPUDCLK0/1/2*/
	put32(GPPUD,0);	//So disable pull up/down
	delay(150);	//Wait 150 cycles, provides required hold time for control signal

	/*Details of the process in page 101 - BCM2835 manual - we want to modify
	function and behaviour of pins 14 and 15 so write to those bits*/
	put32(GPPUDCLK0,(1<<14)|(1<<15));
	delay(150); //Wait 150 cycles, provides required hold time for control signal
	put32(GPPUDCLK0,0); //Then write 0, so remove the clock

	//Important for auxiliary interrupt for user input
	put32(AUX_ENABLES,1);           //Enable mini uart and access to its registers
	put32(AUX_MU_CNTL_REG,0);       //Disable auto flow control and disable
	//receiver and transmitter (for now)
	//RTS request to send, RTR ready to receive, CTS clear to send
	//mini_uart receiver disabled
	//mini_uart transmitter disabled
	//RTS line is controlled by the AUX_MU_MCR_REG register bit 1
	//Transmitter will ignore the status of the CTS line
	//De-assert RTS when the receive FIFO has 3 empty spaces left.
	//RTS auto flow assert level is high*
	//CTS auto flow assert level is high*

	put32(AUX_MU_IER_REG, IER_REG_VALUE);   /*Enable interrupts for FIFO with at
	least 1 byte*/
	put32(AUX_MU_LCR_REG, 3);        //Enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0);        //Set RTS line to be always high

	put32(AUX_MU_BAUD_REG, baudrate_reg);

	/*Set baud rate to 115200
	The baud rate is the rate at which information is transferred in a
	communication channel. Maximum in serial is 115200 bits/sec
	How to calculate baudrate = system_clock_freq / (8 * ( baudrate_reg + 1 ))
	115200 = 250M / (8 * ( baudrate_reg + 1 )) which is
	baudrate_reg = 270,2673611
	It uses 8 times oversampling
	BCM2835 manual, page 11*/

	put32(AUX_MU_IIR_REG, CLEAR_FIFO);//Clear FIFO
	put32(AUX_MU_CNTL_REG,3);       	//Finally, enable transmitter and receiver
}

//After setting the mini_uart we can try it sending and receiving data
void uart_send (char c)
{
	//Start with an infinite loop and use the Line status reg
	while(1)
	{
		/*AND with bit 5 as it tells us that the transmit FIFO can accept at least
		one byte, so we can put the character in the register or write*/
		if(get32(AUX_MU_LSR_REG)&0x20)
			break;
	}
	put32(AUX_MU_IO_REG,c);
	//Put the character in register used for read/write data to/from uart FIFO
}

char uart_recv (void)
{
	while(1)
	{
		/*AND with bit 1 as it tells us that the FIFO holds at least 1 symbol
		or in other terms the data is ready*/
		if(get32(AUX_MU_LSR_REG)&0x01)
			break;
	}
	return(get32(AUX_MU_IO_REG)&0xFF); //Filter out the other bits as are reserved
}

//Funtion to send strings rather than characters
void uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++)
	{
		uart_send((char)str[i]);
	}
}

// This function is required by printf function
void putc (void* p, char c)
{
	uart_send(c);
}

//Handler for user input interrupt
void handle_uart_irq(void)
{
	//There may be more than one byte in the FIFO.
	while((get32(AUX_MU_IIR_REG) & IIR_REG_REC_NON_EMPTY)
					== IIR_REG_REC_NON_EMPTY)
	{
		uart_send(uart_recv());
	}
}
