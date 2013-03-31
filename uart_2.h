// uart2.h


#ifndef _UART_H_
#define _UART_H_

// Definitions
void          uart_init(void);

void          uart_putc(unsigned char c);
unsigned char uart_getc(void);

void          uart_puts(char* str);
void          uart_gets(char* str, int length);

#endif
