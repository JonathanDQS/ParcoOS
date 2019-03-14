/*Works with the mini_uart Universal Asynchronous Receiver/Transmitter for
serial communication*/
#include "printf.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(void)
{
	//Initiates the memory locations for the mini_uart
	uart_init(115200);

	//Initialize printf routine
	init_printf(0, putc);

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
