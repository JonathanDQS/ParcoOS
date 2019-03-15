#ifndef	_P_BASE_H
#define	_P_BASE_H

//The Raspberry Pi 3 reserves the memory above address 0x3F000000 for devices
#define PBASE 0x3F000000
//Local peripherals interruptbase, Quad A7 control page 3
#define LPBASE 0x40000000

#endif  /*_P_BASE_H */
