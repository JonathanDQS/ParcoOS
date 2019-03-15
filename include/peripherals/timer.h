#ifndef	_P_TIMER_H
#define	_P_TIMER_H

#include "peripherals/base.h"

//Manual addresses, BCM2835 page 172
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

#endif  /*_P_TIMER_H */
