#ifndef  _P_IRQ_H
#define  _P_IRQ_H

#include "peripherals/base.h"

//Addresses for interrupts given by the BCM2835 manual, page 112
#define IRQ_BASIC_PENDING             (PBASE+0x0000B200)

#define IRQ_PENDING_1                 (PBASE+0x0000B204)
#define IRQ_PENDING_2                 (PBASE+0x0000B208)
#define FIQ_CONTROL                   (PBASE+0x0000B20C)

//IRQs in 2 registers, BCM2835 manual, page 116
#define ENABLE_IRQS_1                 (PBASE+0x0000B210)
#define ENABLE_IRQS_2                 (PBASE+0x0000B214)

#define ENABLE_BASIC_IRQS             (PBASE+0x0000B218)
#define DISABLE_IRQS_1                (PBASE+0x0000B21C)
#define DISABLE_IRQS_2                (PBASE+0x0000B220)
#define DISABLE_BASIC_IRQS           	(PBASE+0x0000B224)

//Special interupt bits table in page 113, BCM2835 Manual

//Timer interrupts we want to enable, specially 1 and 3 as 0 and 2 are for GPU
//These run at 1MHz
#define SYSTEM_TIMER_IRQ_0           	(1 << 0)
#define SYSTEM_TIMER_IRQ_1           	(1 << 1)
#define SYSTEM_TIMER_IRQ_2           	(1 << 2)
#define SYSTEM_TIMER_IRQ_3           	(1 << 3)

//Use auxiliar interupt to handle user input
#define AUX_IRQ                       (1 << 29)

//Core 0 interupt source address, QA7 page 7
#define CORE0_INTERRUPT_SOURCES       (LPBASE+0x60)

/*In the CORE0_INTERRUPT_SOURCES address bit 11 is the one for Local timer
interupt, QA7 page 16*/
#define LTIMER_INTERRUPT            	(1 << 11)

#endif  /*_P_IRQ_H */
