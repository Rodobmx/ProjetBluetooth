// 
// Filename : uart.c
// Author   : KevinM
// Modified : 05/04/2012
//


// Includes
#include "uart.h"
#include <io430g2553.h>
#include <stdint.h>


// Defines
#define RXD BIT1
#define TXD BIT2


// Variables
//unsigned int  tx_flag;			// Flag indicate that a byte is sending.
//unsigned char tx_char;			//

unsigned int  rx_flag;			// Flag indicate that a byte is receiving.
unsigned char rx_char;			//


// Definitions
// ------------------------------------
// Function    : uart_init
// Description : Set up the UART interface via USCI
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void uart_setup(uint8_t baudrate)
{
  //int val;
  
  IE2 |= UCA0RXIE;			// Disable USCI_A0 RX interrupt
  UCA0CTL1 &= ~UCSWRST;			// Put UCSWRST offline for reset
  
  P1SEL  |= (RXD + TXD);                // Set up the I/O
  P1SEL2 |= (RXD + TXD);                // TXD and RXD
  
  switch(baudrate)
  {
    case 96:    // 9600 Baud
      UCA0CTL1 |= UCSSEL_2;             // SMCLK
      
      //val = 1000000/9600;             // 1 000 000 Hz, 9600 Baud, UCBR1=0x68, UCBR0=0x00
      //UCA0BR0 = val & 0xFF;           // 1MHz, 9600
      //UCA0BR1 = val >> 8;             // 1MHz, 9600
      UCA0BR0 = 104;			// 1 MHz, 9600 -> 104.1
      UCA0BR1 = 0;			// 1 MHz, 9600
    break;
    
    case 11:    // 115 200 Baud
      // TODO
      UCA0BR0 = 9;			// 1 MHz, 9600 -> 8.6
      UCA0BR1 = 0;			// 1 MHz, 9600
    break;
    
    default:
    break;
  }
  /*
  UCA0CTL0 &= ~UCPEN;                   // UCPEN : Parity enable/disable ;
  UCA0CTL0 &= ~UCPAR;                   // UCPAR : nb parity bits
  UCA0CTL0 &= ~UCMSB;                   // UCMSB : MSB/LSB first (LSB first)
  
  UCA0CTL0 &= ~UC7BIT;                  // UC7BIT : nb data bits (8 bits)
  UCA0CTL0 &= ~UCSPB;                   // UCSPB :  nb stop bits (1 bit)
  */
  UCA0CTL0 &= ~(UCPEN + UCPAR + UCMSB + UC7BIT + UCSPB + UCMODE0);
  UCA0CTL1 |= UCSSEL_2; // Choix de la clock à 1 Mhz
  
  UCA0CTL1 &= ~UCSWRST ;                // USCI state machine , desactivation du reset logiciel
  IE2 |= UCA0RXIE;                      // Enable USCI_A0 RX interrupt
  
  rx_flag = 0;				// Set rx_flag to 0
  
}

// ------------------------------------
// Fonction    : uart_putc
// Description : Sends a char to the UART. Will wait if the UART is busy
// Param(s)    : unsigned char c, char to send
// Output      : uint8_t , nb char send
// ------------------------------------
uint8_t uart_putc(unsigned char c)
{
/*
  while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
*/
  while(UCA0STAT & UCBUSY);
  UCA0TXBUF = c; 
  
/*      Une interruption sert à detecté un changement extérieur, à l'interieur du soft elle te sert à rien.
        Comme sa tu perd du temps et tu augmente la taille du code bêtement.
  tx_char = c;                  // Put the char into the tx_char
  IE2 |= UCA0TXIE; 		// Enable USCI_A0 TX interrupt
  while(tx_flag == 1);		// Have to wait for the TX buffer
  tx_flag = 1;			// Reset the tx_flag
  */
  return 1;
}

// ------------------------------------
// Fonction    : uart_puts
// Description : Sends a string to the UART. Will wait if the UART is busy
// Param(s)    : char* str, pointer to string to send
// Output      : uint8_t , nb char send
// ------------------------------------
uint8_t uart_puts(char* str)
{
  uint8_t i = 0;
  while(str[i] != '\0')
  {
    uart_putc(str[i++]);
    /* Cette ligne ne doit pas apparaitre la, mais dans le putc, il n'y à aucune vérification dans le putc (enfin maintenant si) */
    //while(UCA0STAT & UCBUSY);	// Wait until the last byte is completely sent
  }
  return i;
}

// ------------------------------------
// Fonction    : uart_getc
// Description : Get a char from the UART.
// Param(s)    : unsigned char* car, pointer to store the caracter received 
// Output      : unsigned char, char from UART
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
// Fonction    : uart_gets
// Description : Get a string of known length from the UART. Strings terminate when enter is pressed or string buffer fills
//               Will return when all the chars are received or a carriage return (\r) is received. Waits for the data.
// Param(s)    : char* str, array pointer
//               int length, length of the string
// Output      : Nothing
// ------------------------------------
uint8_t uart_gets(char* str, int length)
{
  uint8_t i = 0;
  
  for(i=0; i< length; i++)
    str[i] = uart_getc();               // Recupération du caractere
  
  str[i] = '\0';
  
  return i;
}

// ------------------------------------
// Fonction    : uart_gets_until
// Description : ...
//               ...
// Param(s)    : ...
//               ...
// Output      : ...
// ------------------------------------
uint8_t uart_gets_until(char* str, char stopch, uint8_t maxLength)
{
  uint8_t i;
  char c;

  for(i = 0; i < (maxLength); i++)
  {
    c = uart_getc();
    str[i] = c;
    
    if (c == stopch){
      i++;
      break;	// we're done if stopchar is found
    }  
  }
  str[i] = '\0';      // "++i" pour garder le stopch

  return i;
}

// ------------------------------------
// Fonction    : uart_gets_startWithAndUntil
// Description : ...
//               ...
// Param(s)    : ...
//               ...
// Output      : ...
// ------------------------------------
uint8_t uart_gets_startWithAndUntil(char* str, char startch, char stopch, uint8_t maxLength)
{
  uint8_t i;
  char c;/* Pourquoi l'utilisation d'une variable supplémentaire ? (double les affectations) */

  do
  {
    c = uart_getc();
  }while(c != startch);
  
  str[0] = c;
  
  for(i = 1; i < (maxLength); i++)
  {
    c = uart_getc();
    str[i] = c;
    
    if (c == stopch){
      //i++;
      break;	// we're done if stopchar is found
    }
  }
  //str[i] = '\0'; On ne traite pas des string, innutile d'avoir un '\0' en fin de trame

  return i;
}

// ------------------------------------
// UART TX interrupt routine.
// ------------------------------------
/* Interruption sur un envoi inutile
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
  rx_char = (char)UCA0RXBUF;	// Copy from RX buffer, in doing so we ACK the interrupt as well
  rx_flag = 1;			// Set the rx_flag to 1
}
