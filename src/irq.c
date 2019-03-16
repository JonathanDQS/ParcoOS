#include "utils.h"
#include "printf.h"
#include "timer.h"
#include "entry.h"
#include "peripherals/irq.h"
#include "mini_uart.h"

//Given a non-handled interrupt print which one was it with help of this array
const char *entry_error_messages[] =
{
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",
	"FIQ_INVALID_EL1t",
	"ERROR_INVALID_EL1T",

	"SYNC_INVALID_EL1h",
	"IRQ_INVALID_EL1h",
	"FIQ_INVALID_EL1h",
	"ERROR_INVALID_EL1h",

	"SYNC_INVALID_EL0_64",
	"IRQ_INVALID_EL0_64",
	"FIQ_INVALID_EL0_64",
	"ERROR_INVALID_EL0_64",

	"SYNC_INVALID_EL0_32",
	"IRQ_INVALID_EL0_32",
	"FIQ_INVALID_EL0_32",
	"ERROR_INVALID_EL0_32"
};

//This function sets the appropriate register and enables the timer interrupt
void enable_interrupt_controller()
{
	put32(ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1);
}

//Message for "invalid" interrupts
void show_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
	printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr, address);
}

//Method called to handle the interrupts
void handle_irq(void)
{
	//This could be done better with a jump table
	unsigned int irqCore = get32(IRQ_PENDING_1);
	while (irqCore)
	{
		if (irqCore & SYSTEM_TIMER_IRQ_1)
		{
			handle_timer_irq();
			irqCore &= ~SYSTEM_TIMER_IRQ_1;
		}
		else if (irqCore & AUX_IRQ)
		{
			handle_uart_irq();
			irqCore &= ~AUX_IRQ;
		}
		else
		{
			printf("Unknown pending irq core specific: %x\r\n", irqCore);
		}
	}

	unsigned int irq = get32(CORE0_INTERRUPT_SOURCES);
	switch (irq)
	{
		case (LTIMER_INTERRUPT):
			handle_local_timer_irq();
			break;
		default:
			printf("Unknown pending irq: %x\r\n", irq);
	}
}
