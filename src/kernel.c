/*Works with the mini_uart Universal Asynchronous Receiver/Transmitter for
serial communication*/
#include "timer.h"
#include "printf.h"
#include "irq.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(void)
{
	//Initiates the memory locations for the mini_uart
	uart_init(115200);

	//Initialize printf routine
	init_printf(0, putc);
	//Call the routine to set the vector, handlers
	irq_vector_init();
	//Then set the timer interrupt compare value
	timer_init();
	//Local timer interupt initialize
	local_timer_init();
	//Enable the timer interrupt to be triggered
	enable_interrupt_controller();
	//Enable interrupt bit in the processor register
	enable_irq();

	//Print exception level
	int el = get_el();
	printf("Exception level: %d \r\n", el);

	//Then for everything send to it, it is printed to the screen
	while (1)
	{
			uart_send(uart_recv());
	}
}

//Use another core to print a message with a delay
void kernel_main1(void)
{
	//Do the same as the previous but with a little delay
	delay(1000000);
	int el = get_el();
	printf("Exception level: %d \r\n", el);
	uart_send_string("Hello, World!\r\n");
}
