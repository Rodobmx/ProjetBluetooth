// uart.c


// Includes
#include <io430g2553.h>
#include "uart.h"


// Defines
#define RXD BIT1
#define TXD BIT2


// Variables
unsigned int  tx_flag;			// Flag indicate that a byte is sending.
unsigned char tx_char;			//

unsigned int  rx_flag;			// Flag indicate that a byte is receiving.
unsigned char rx_char;			//


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
  
  UCA0CTL0 &= ~UCPEN & ~UCPAR;          // desactivation de la parit� ; parit� "odd"
  UCA0CTL0 &= ~UCMSB;                   // determination de l'ordre des bits (LSB en 1er)
  
  UCA0CTL0 &= ~UC7BIT;                  // determination de la longueur des donn�es (8 bits)
  UCA0CTL0 &= ~UCSPB;                   // determination du nb de bit de stop (1 bit)
  
  UCA0CTL1 &= ~UCSWRST ;                // USCI state machine , desactivation du reset logiciel
  
  IE2 |= UCA0RXIE;                      // Enable USCI_A0 RX interrupt
  
  rx_flag = 0;				// Set rx_flag to 0
  tx_flag = 0;				// Set tx_flag to 0
  
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
/*
  while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
*/
	
  tx_char = c;                  // Put the char into the tx_char
  IE2 |= UCA0TXIE; 		// Enable USCI_A0 TX interrupt
  while(tx_flag == 1);		// Have to wait for the TX buffer
  tx_flag = 1;			// Reset the tx_flag
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
    while(UCA0STAT & UCBUSY);	// Wait until the last byte is completely sent
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
/*
  while (!(IFG2 & UCA0RXIFG));
    return (char)UCA0RXBUF;
*/
  
  while (rx_flag == 0);		// Wait for rx_flag to be set
  rx_flag = 0;			// ACK rx_flag
  
  return rx_char;
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

// ------------------------------------
// Function : uart_gets_until
//
// Description : 
//               
//
// Param(s) : char* str, array pointer
//            char stopch, TODO ...
//            int length, length of the string
//
// Return   : uint8_t, nb of caracters read
// ------------------------------------
uint8_t uart_gets_until(char* str, uint8_t len, char stopch)
{
  // TODO
  uint8_t i, count;
  char c;

  count = 0;
  for (i = 0; i < (len - 1); i++)
  {
    c = uart_getc();
    str[i] = c;
    count++;
    if (c == stopch)
      break;
  }
  str[(++i)] = '\0';

  return count;
}

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

// ------------------------------------
// UART RX interrupt routine.
// ------------------------------------
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  rx_char = (char)UCA0RXBUF;	// Copy from RX buffer, in doing so we ACK the interrupt as well
  rx_flag = 1;			// Set the rx_flag to 1
}
