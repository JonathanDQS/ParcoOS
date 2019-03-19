#ifndef	_MINI_UART_H
#define	_MINI_UART_H

//Define functions to be implemented to help initiate and use the Mini UART
void uart_init (unsigned int baudrate);
char uart_recv (void);
void uart_send (char c);
void uart_send_string(char* str);
void putc (void* p, char c);
void handle_uart_irq (void);

#endif  /*_MINI_UART_H */
