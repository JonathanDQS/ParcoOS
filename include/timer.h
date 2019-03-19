#ifndef	_TIMER_H
#define	_TIMER_H

//Declare functions to be implemented to set and handle timer interrupts

//Timer interupt
void timer_init (void);
void handle_timer_irq (void);

/*Local timer interupt - more realiable - always takes its pulses from the
crystal*/
void local_timer_init (void);
void handle_local_timer_irq (void);

#endif  /*_TIMER_H */
