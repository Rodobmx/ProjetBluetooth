// system.c


// Includes
#include <io430g2553.h>
#include <intrinsics.h>

#include "system.h"


// Definitions
// ------------------------------------
//
// ------------------------------------
void board_init(void)
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
  
  __bis_SR_register(GIE);      // interrupts enabled
}

// ------------------------------------
// Fonction permettant d'effectuer une tempo de x ms
// ------------------------------------
void tempo_ms(int ms)
{
  for(int i=0 ; i<ms; i++)
  {
    __delay_cycles(1000);
  }
}
