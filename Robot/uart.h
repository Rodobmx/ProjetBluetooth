//
// Filename : uart.h
// Author   : KevinM
// Modified : 05/04/2012
//


#ifndef _UART_H_
#define _UART_H_


// Includes
#include <stdint.h>


// Definitions
void          uart_setup(uint8_t baudrate);

uint8_t       uart_putc(unsigned char c);
unsigned char uart_getc(void);

uint8_t       uart_puts(char* str);
uint8_t       uart_gets(char* str, int length);
uint8_t       uart_gets_until(char* str, char stopch, uint8_t maxLength);
uint8_t       uart_gets_startWithAndUntil(char* str, char startch, char stopch, uint8_t maxLength);

__interrupt void USCI0TX_ISR(void);
__interrupt void USCI0RX_ISR(void);

#endif
