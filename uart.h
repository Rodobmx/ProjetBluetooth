// uart.h


#ifndef _UART_H_
#define _UART_H_


// Includes
#include <stdint.h>


// Definitions
void          uart_init(void);

void          uart_putc(unsigned char c);
unsigned char uart_getc(void);

void          uart_puts(char* str);
void          uart_gets(char* str, int length);
uint8_t uart_gets_until(char* str, uint8_t len, char stopch);

__interrupt void USCI0TX_ISR(void);
__interrupt void USCI0RX_ISR(void);

#endif
