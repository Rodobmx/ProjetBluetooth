/*
	SAVOURET RODOLPHE
	TRANIELLO SEBASTIEN
*/
#include "uart.h"

/**
  * Initialisation de la config de l'uart
  * 9600bauds, pas de parité, 8 bits de data
  */
void UART_Init()
{
  
  P1SEL |= BIT1 + BIT2; // P1.1 et P1.2 UART MODE TX / RX
  P1SEL2 |= BIT1 + BIT2; // P1.1 et P1.2 UART MODE
  
  UCA0BR0 = 9; // 1 000 000 Hz / 115200 bauds = 8.6
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


/**
  * Fonction sous interuption lancé à la réception d'un caracère
  * Interprete directement le caractere reçu 
  */
  /*
#pragma vector = USCIAB0RX_VECTOR // Vecteur d'interruption à la reception sur l'UART
__interrupt void USCI0RX_ISR (void) 
{
  if(IFG2 & UCA0RXIFG)
  {
    char c = UART_GetChar(); // récupère le caractere reçu
    switch(c) // interprete la commande en fonction du caractere
    {
      
      case 'H':
      case 'h':
        UART_PutString("Help \r\n   - v : version\r\n   - l allumer led\r\n   - e eteindre led\r\n", sizeof("Help :\r\n   - v : version\r\n   - l allumer led\r\n   - e eteindre led\r\n"));
        break;
        
      case 'V':
      case 'v':
        UART_PutString("Version : 1.0\r\n", sizeof("Version : 1.0\r\n"));
        break;
        
      case 'L':
      case 'l':
        UART_PutChar(c);
        P1OUT |= BIT6;
        break;
        
      case 'e':
      case 'E':       
        P1OUT &= ~BIT6; 
        break;
	default: // caractere non reconnus.
		break;
    }
  }
  IFG2 = 0; // remise a 0 du flag d'interruption (+/- nécessaire en fonction du µC).
}
*/