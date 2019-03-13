#ifndef	_MINI_UART_H
#define	_MINI_UART_H

void uart_init_115200 ( void );
void uart_init (unsigned int baudrate );
char uart_recv ( void );
void uart_send ( char c );
void uart_send_1 ( char c );
void uart_send_string(char* str);

#endif  /*_MINI_UART_H */
