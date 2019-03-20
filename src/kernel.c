/*Works with the mini_uart Universal Asynchronous Receiver/Transmitter for
serial communication*/
#include "timer.h"
#include "printf.h"
#include "irq.h"
#include "utils.h"
#include "mini_uart.h"

//Process to prove scheduler works, must give it a "string" of 5 chars
void process(char *array)
{
	while (1)
	{
		for (int i = 0; i < 5; i++)
		{
			uart_send(array[i]);
			delay(100000);
		}
	}//Keep in this process till we have another one to run or interrupt
}

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

	//Make sure scheduling works by copying a process with different arguments
	int res = copy_process((unsigned long)&process, (unsigned long)"12345");
	//Check the process was added to the queue /////////////////
	if (res != 0)
	{
		printf("Error while starting process 1");
	  return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"abcde");
	//Check the process was added to the queue /////////////////
	if (res != 0)
	{
		printf("Error while starting process 2");
	  return;
	}

	while (1)
	{
		//Call so this process can relinquish control and let other take over
		schedule();
	}

	//Then for everything send to it, it is printed to the screen
	// while (1)
	// {
	// 	uart_send(uart_recv());
	// }
	//NOT POLLING ANYMORE
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
