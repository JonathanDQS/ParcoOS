#ifndef	_P_TIMER_H
#define	_P_TIMER_H

#include "peripherals/base.h"

//Manual addresses, BCM2835 page 172
//System timer running at 1MHz
//Control/status memory address
#define TIMER_CS        (PBASE+0x00003000)
//Counter lower 32 bits
#define TIMER_CLO       (PBASE+0x00003004)
//Counter upper 32 bits
#define TIMER_CHI       (PBASE+0x00003008)
//Timer compares
#define TIMER_C0        (PBASE+0x0000300C)
#define TIMER_C1        (PBASE+0x00003010)
#define TIMER_C2        (PBASE+0x00003014)
#define TIMER_C3        (PBASE+0x00003018)

//In control status address we check which comparator is matched
#define TIMER_CS_M0	(1 << 0)
#define TIMER_CS_M1	(1 << 1)
#define TIMER_CS_M2	(1 << 2)
#define TIMER_CS_M3	(1 << 3)

//Quad A7, page 7
/*This local timer can generate interrupts and always gets its pulses from the
crystal clock, 38,4MHz, it has a 28 bit programmable divider
Automatically counts down and reloads when it gets to 0, also set interrupt flag
when it happens, user must clear interrupt flag, "counter" stills run but
no detection if the flag has not been cleared*/
//Local timer control and status address of timer interupt "sent" to core 0
#define LTIMER_CTRL    (LPBASE+0x34)

//Local timer write flags - address where we write
#define LTIMER_CLR     (LPBASE+0x38)

/*Bits for the timer control address:
31 Read only interupt flag
29 Interrup enable - active high
28 Timer enabled - active high
27:0 Re-load value*/

/*Bits for the time clear and reload, only write bits
31 Interrupt flag clear when 1 written
30 Local timer-reloaded when 1 written - if written, timer reloaded without
  interrupt generated
27:0 Unused*/

//Enable timer
#define LTIMER_CTRL_EN     (1 << 28)
//Enable interupt
#define LTIMER_CTRL_INT_EN (1 << 29)
//Value to be written to the Timer control address
#define LTIMER_CTRL_VALUE  (LTIMER_CTRL_EN | LTIMER_CTRL_INT_EN)

//Acknowledge to be written so interrupt handled
#define LTIMER_CLR_ACK (1 << 31)

#endif  /*_P_TIMER_H */
