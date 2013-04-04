//robot.c

// Includes
#include "robot.h"
#include "led.h"
#include <io430g2553.h>


// Definitions
// --------------------------
// Fonction : 
// Description : 
// Param(s) : 
// Output : 
// --------------------------
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
                    led_green_on();
                  else
                    led_green_off();
                  
                  if(buffer[TRAME_LEDD_INDEX])
                     led_red_on();
                  else
                    led_red_off();
                break;
                	
            default:
		break;
		
	}
}