// 
// Filename : system.c
// Author   : KevinM
// Modified : 05/04/2012
//


// Includes
#include "system.h"
#include <io430g2553.h>


// Definitions
// ------------------------------------
// Fonction    : board_init
// Description : Permet d'initialiser la carte
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void board_setup(void)
{
  // Arret du watchdog
  WDTCTL = WDTPW + WDTHOLD;
  
  // DCO calibration at 1MHz
  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
  {  
    while(1);                               // If calibration constants erased
                                            // do not load, trap CPU!!
  } 
  //1Mhz
  BCSCTL1 = CALBC1_1MHZ;                    // Set range
  DCOCTL  = CALDCO_1MHZ;                    // Set DCO step + modulation  
  
  __enable_interrupt();                     // Enable interrupts
}

// ------------------------------------
// Fonction    : tempo_ms
// Description : Permet d'effectuer une tempo de x ms
// Param(s)    : int ms, la durée en ms
// Output      : Nothing
// ------------------------------------
void tempo_ms(int ms)
{
  for(int i=0 ; i<ms; i++)
  {
    __delay_cycles(1000);
  }
}
