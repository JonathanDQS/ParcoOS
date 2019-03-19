#include "utils.h"
#include "printf.h"
#include "peripherals/timer.h"

//Refering to interval variable as frequency in a core can vary
const unsigned int intervalVar = 1000000;
const unsigned int interval = 0x8000000;
unsigned int curVal = 0;
unsigned int seconds = 0;
unsigned int secondsAc = 0;

//Initialize timer by getting counter lower 32 bits
void timer_init ( void )
{
	curVal = get32(TIMER_CLO);
	curVal += intervalVar;			 //add the interval we wanto to use
	put32(TIMER_C1, curVal); //and set the comparator
}

//Once the interrupt is raised
void handle_timer_irq( void )
{
	//Take previous value which triggered interrupt and set the next interval
	curVal += intervalVar;
	put32(TIMER_C1, curVal);
	//Acknowledge interrupt handled
	put32(TIMER_CS, TIMER_CS_M1);
	printf("TimInter: %d\n\r", seconds);
	seconds += 1;
}

//Initialize local timer
void local_timer_init ( void )
{
	/*Write to the Local timer control and status address the value to "count" and
	enable it, this is the one which is "sent" to core 0*/
	put32(LTIMER_CTRL, (interval | LTIMER_CTRL_VALUE));
}

//Handler of the local timer
void handle_local_timer_irq(void)
{
	printf("TimInterAcc: %d\n\r", secondsAc);
	secondsAc += 1;
	put32(LTIMER_CLR, LTIMER_CLR_ACK);
}
