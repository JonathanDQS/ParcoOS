/*Works with the mini_uart Universal Asynchronous Receiver/Transmitter for
serial communication*/
#include "mini_uart.h"
#include "utils.h"

void kernel_main(void)
{
	//Initiates the memory locations for the mini_uart
	uart_init(115200);

	//Then for everything send to it, it is printed to the screen
	while (1)
	{
			uart_send(uart_recv());
	}
}

//Use another core to print a message with a delay
void kernel_main1(void)
{
	delay(1000000);
	uart_send_string("Hello, World!\r\n");
}
