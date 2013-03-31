//robot.c

// Includes
#include "robot.h"
#include <io430x16x.h>


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
