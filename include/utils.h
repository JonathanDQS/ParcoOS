#ifndef	_BOOT_H
#define	_BOOT_H

//Declaration of functions implemented to do simple but crucial operations
extern void delay (unsigned long);
extern void put32 (unsigned long, unsigned int);
extern unsigned int get32 (unsigned long);
extern int get_el (void);

#endif  /*_BOOT_H */
