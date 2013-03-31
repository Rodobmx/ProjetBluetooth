/*
	SAVOURET RODOLPHE
	TRANIELLO SEBASTIEN
*/
#include "uart.h"
#include "robot.h"

#define TRAME_CAR_START '-'             // Caractere debut de trame
#define TRAME_CAR_SEPARCMD ':'          // caractere separation commande
#define TRAME_CAR_END ';'               // caractere fin de trame
#define TRAME_CMD_INDEX 1               // index du caractere de commande
#define TRAME_SENSA_INDEX 3             // index signe roue A
#define TRAME_SENSB_INDEX 8             // index signe roue B
#define TRAME_CAR_SEPAR ','


#define TRAME_CAR_SENS_POS '+'
#define TRAME_CAR_SENS_NEG '-'

#define TRAME_CAR_VITESSE 'v'   // -v:+100,+100;
#define TRAME_CAR_LED 'l'       // -l:g,d;
#define TRAME_LEDG_INDEX 3             // index led gauche
#define TRAME_LEDD_INDEX 5             // index led droite
#define TRAME_CAR_AU 's'        // Arret Urgence




unsigned char buffer[15];
unsigned int index=0;

/*............................................*/
/* Fonction permettant d'allumer la led verte */
/*............................................*/
void allumer_led_verte( void )
{
  P1OUT |= BIT6; 
}

/*.............................................*/
/* Fonction permettant d'?teindre la led verte */
/*.............................................*/
void eteindre_led_verte( void )
{
  P1OUT &= ~BIT6; 
}

/*............................................*/
/* Fonction permettant d'allumer la led rouge */
/*............................................*/
void allumer_led_rouge( void )
{
  P1OUT |= BIT0; 
}

/*.............................................*/
/* Fonction permettant d'?teindre la led rouge */
/*.............................................*/
void eteindre_led_rouge( void )
{
  P1OUT &= ~BIT0; 
}

void GetTrame()
{
	index = 0;
	buffer[index] = UART_GetChar();
	
	while(buffer[index] != TRAME_CAR_START)
		buffer[index] = UART_GetChar();;
	
	while(buffer[index] != TRAME_CAR_END)
	{
		index ++;
		buffer[index] = UART_GetChar();
	}
}
/*
TACCR1 gauche
TACCR2 droite

*/
void parseTrame()
{
	switch(buffer[TRAME_CMD_INDEX])
	{
              case TRAME_CAR_VITESSE:
                  {
			unsigned short roueA = 0, roueB = 0;
			if(buffer[TRAME_SENSA_INDEX] == TRAME_CAR_SENS_NEG)
				roueA = 1;
			if(buffer[TRAME_SENSB_INDEX] == TRAME_CAR_SENS_NEG)
				roueB = 1;
			setSens(roueA,roueB);
			
			unsigned vitesseA=0, vitesseB=0;
			vitesseA = (buffer[TRAME_SENSA_INDEX+1]-48)*100 +(buffer[TRAME_SENSA_INDEX+2]-48)*10 + (buffer[TRAME_SENSA_INDEX+3]-48);
			vitesseB = (buffer[TRAME_SENSB_INDEX+1]-48)*100 +(buffer[TRAME_SENSB_INDEX+2]-48)*10 + (buffer[TRAME_SENSB_INDEX+3]-48);
			
			TACCR1 = vitesseA*10;
			TACCR2 = vitesseB*10;
                  }			
		break;
		
              case TRAME_CAR_AU:
                        TACCR1 = 0;
			TACCR2 = 0;
		break;
                
              case TRAME_CAR_LED:
                  if(buffer[TRAME_LEDG_INDEX])
                    allumer_led_verte();
                  else
                    eteindre_led_verte();
                  
                  if(buffer[TRAME_LEDD_INDEX])
                    allumer_led_rouge();
                  else
                    eteindre_led_rouge();
                break;
                	
            default:
		break;
		
	}
}

void setSens(short roueA, short roueB) // 0 avancer, 1 reculer   P1OUT &= ~BIT6; 
{
  if(roueA)
    P2OUT |= BIT1;
  else
    P2OUT &= ~BIT1;
      
  if(roueB)
    P2OUT |= BIT2;
  else
    P2OUT &= ~BIT2;
}

