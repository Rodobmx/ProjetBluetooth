
#include "io430.h"

#ifndef _UART_H_
#define _UART_H

void UART_Init();
void UART_PutChar(char buff);
char UART_GetChar();

#endif