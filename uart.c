/*
	SAVOURET RODOLPHE
	TRANIELLO SEBASTIEN
*/
#include "uart.h"
#include "robot.h"

#include <io430g2553.h>

unsigned char buffer[15]="-s:+050,+050;";
unsigned int index=0;

/**
  * Initialisation de la config de l'uart
  * 115200 bauds, pas de parité, 8 bits de data
  */
void UART_Init()
{
  P1DIR |= BIT2;
  P1SEL |= BIT1 + BIT2; // P1.1 et P1.2 UART MODE TX / RX
  P1SEL2 |= BIT1 + BIT2; // P1.1 et P1.2 UART MODE
  
  //UCA0BR0 = 9; // 1 000 000 Hz / 115200 bauds = 8.6
  UCA0BR0 = 104; // 1 000 000 Hz / 9600 bauds = 104.1
  UCA0BR1 = 0;
  
  // UCPEN : Parity enable/disable 
  // UCPAR nb bit de parité 
  // UCMSB MSB/LSB First
  // UC7BIT : Nb de bit de data
  // le ~ inverse les bits.
  UCA0CTL0 &= ~(UCPEN + UCPAR + UCMSB + UC7BIT + UCSPB + UCMODE0);
  UCA0CTL1 |= UCSSEL_2; // Choix de la clock à 1 Mhz
  //IFG2 = 0;

  UCA0CTL1 &= ~UCSWRST; // Desactive le RST de l'UART
  IE2 |= UCA0RXIE; // Interrupt Enable (Reception sur l'UART)
}

/**
  * Fonction permettant l'envoi d'un élément
  * buff : caractere à envoyer
  */
void UART_PutChar(char buff)
{
  while(UCA0STAT & UCBUSY);
  UCA0TXBUF = buff;
}

/**
  * Fonction permettant la reception d'un caractère
  * Return : caractere recu
  */
char UART_GetChar()
{
	while (!(IFG2 & UCA0RXIFG));
	return UCA0RXBUF;
}


/**
  * Fonction permettant l'envoi d'une chaine de caractere
  * buff : pointeur sur chaine de caractere
  * size : nombre de caractere dans la chaine
  */
void UART_PutString(char* buff, unsigned int size)
{
  unsigned int i;
  for(i=0; i<size-1; i++)
  {
    UART_PutChar(buff[i]);
  }
}

void GetTrame()
{
	index = 0;
	buffer[index] = UART_GetChar();
	
	while(buffer[index] != TRAME_CAR_START) // Start with -
		buffer[index] = UART_GetChar();
	
	while(buffer[index] != TRAME_CAR_END)
	{
		index ++;
		buffer[index] = UART_GetChar();
	}
        
        char p = '>';
        UART_PutChar(p);
}