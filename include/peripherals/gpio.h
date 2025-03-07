#ifndef	_P_GPIO_H
#define	_P_GPIO_H

//Rely on base and offsets
#include "peripherals/base.h"

//GPIO function select 1
//BCM2835 manual - page 91
//General purpose I/0 function select registers 1
#define GPFSEL1         (PBASE+0x00200004)
//GPIO pin output set 0
#define GPSET0          (PBASE+0x0020001C)
//GPIO pin output clear 0
#define GPCLR0          (PBASE+0x00200028)
//GPIO pin pull up-down enable 0
#define GPPUD           (PBASE+0x00200094)
//GPIO pin pull up-down enable clock 0
#define GPPUDCLK0       (PBASE+0x00200098)

#endif  /*_P_GPIO_H */
