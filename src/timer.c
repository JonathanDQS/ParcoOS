#include "utils.h"
#include "printf.h"
#include "peripherals/timer.h"

const unsigned int interval = 1000000;
unsigned int curVal = 0;
unsigned int seconds = 0;

//Initialize timer by getting counter lower 32 bits
void timer_init ( void )
{
	curVal = get32(TIMER_CLO);
	curVal += interval;			 //add the interval we wanto to use
	put32(TIMER_C1, curVal); //and set the comparator
}

//Once the interrupt is raised
void handle_timer_irq( void )
{
	//Take previous value which triggered interrupt and set the next interval
	curVal += interval;
	put32(TIMER_C1, curVal);
	//Acknowledge interrupt handled
	put32(TIMER_CS, TIMER_CS_M1);
	printf("Timer interrupt received: %d\n\r", seconds);
	seconds += 1;
}
