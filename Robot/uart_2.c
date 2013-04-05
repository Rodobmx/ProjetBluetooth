// uart_2.c


// Includes
#include <io430g2553.h>
#include "uart_2.h"


// Defines
#define RXD BIT1
#define TXD BIT2


// Variables
unsigned char rx_char;


// Definitions
// ------------------------------------
// Function : uart_init
//
// Description : Set up the UART interface via USCI
//
// Param(s) : Nothing
//
// Return   : Nothing
// ------------------------------------
void uart_init(void)
{
  int val;
  
  P1SEL  |= (RXD + TXD);                // Set up the I/O
  P1SEL2 |= (RXD + TXD);                // TXD and RXD
  
  UCA0CTL1 |= UCSSEL_2;                 // SMCLK
  
  val = 1000000/9600;                   // 1 000 000 Hz, 9600 Baud, UCBR1=0x68, UCBR0=0x00
  UCA0BR0 = val & 0xFF;                 // 1MHz, 9600
  UCA0BR1 = val >> 8;                   // 1MHz, 9600
  
  UCA0CTL0 &= ~UCPEN & ~UCPAR;          // desactivation de la parité ; parité "odd"
  UCA0CTL0 &= ~UCMSB;                   // determination de l'ordre des bits (LSB en 1er)
  
  UCA0CTL0 &= ~UC7BIT;                  // determination de la longueur des données (8 bits)
  UCA0CTL0 &= ~UCSPB;                   // determination du nb de bit de stop (1 bit)
  
  UCA0CTL1 &= ~UCSWRST ;                // USCI state machine , desactivation du reset logiciel
  
  IE2 |= UCA0RXIE;                      // Enable USCI_A0 RX interrupt
  
}

// ------------------------------------
// Function : uart_putc
//
// Description : Sends a char to the UART. Will wait if the UART is busy
//
// Param(s) : unsigned char c, char to send
//
// Return   : Nothing
// ------------------------------------
void uart_putc(unsigned char c)
{
  while (!(IFG2 & UCA0TXIFG));          // On reste en attente, tant que l'on est pas près à transmettre
    UCA0TXBUF = c;                      // on envoi le caractere
}

// ------------------------------------
// Function : uart_puts
//
// Description : Sends a string to the UART. Will wait if the UART is busy
//
// Param(s) : char* str, pointer to string to send
//
// Return   : Nothing
// ------------------------------------
void uart_puts(char* str)
{
  while(*str)
  {
    uart_putc(*str++);
    while(UCA0STAT & UCBUSY);	        // Wait until the last byte is completely sent
  }
  
}

// ------------------------------------
// Function : uart_getc
//
// Description : Get a char from the UART.
//
// Param(s) : Nothing
//
// Return   : unsigned char, char from UART
// ------------------------------------
unsigned char uart_getc(void)
{
  while (!(IFG2 & UCA0RXIFG));          // On reste en attente, tant que l'on est pas près à recevoir
    return UCA0RXBUF;             // On récupère le caractere
}

// ------------------------------------
// Function : uart_gets
//
// Description : Get a string of known length from the UART. Strings terminate when enter is pressed or string buffer fills
//               Will return when all the chars are received or a carriage return (\r) is received. Waits for the data.
//
// Param(s) : char* str, array pointer
//            int length, length of the string
//
// Return   : Nothing
// ------------------------------------
void uart_gets(char* str, int length)
{
  unsigned int i = 0;
  
  while(i < length)
  {
	str[i] = uart_getc();
	if(str[i] == '\r')
	{
	  for( ; i< length; i++)
	  {
		str[i] = '\0';
	  }
	}
	i++;
  }
}

/*
// ------------------------------------
// UART TX interrupt routine.
// ------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
  UCA0TXBUF = tx_char;		// Copy char to the TX Buffer
  tx_flag = 0;			// ACK the tx_flag
  IE2 &= ~UCA0TXIE; 		// Turn off the interrupt to save CPU
}
*/

// ------------------------------------
// UART RX interrupt routine.
// ------------------------------------
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  rx_char = UCA0RXBUF;            // Copy from RX buffer
}
