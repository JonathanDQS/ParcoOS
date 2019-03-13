/*Works with the mini_uart Universal Asynchronous Receiver/Transmitter for
serial communication*/
#include "mini_uart.h"
#include "utils.h"

void kernel_main(void)
{
	//Prints hello world in screen
	uart_init(19200);

	//Then for everything send to it, it is printed to the screen
	while (1)
	{

		if (uart_recv() == 'a')
		{
			uart_send_string("Hola, Mundo!\0");
		}
		else
			uart_send(uart_recv());
	}
}

void kernel_main1(void)
{
	delay(1000000);
	uart_send_string("Hello, World!\n\0");
}
