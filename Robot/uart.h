// uart.h
#ifndef _UART_H_
#define _UART_H_


// Includes
#include <stdint.h>

// Definitions

void UART_Init();
void UART_PutChar(char buff);
char UART_GetChar();
void GetTrame();

#endif
