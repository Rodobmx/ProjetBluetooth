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
}

// --------------------------
// Fonction : 
// Description : 
// Param(s) : 
// Output : 
// --------------------------
void initTimer(void)
{
  // 0 dec +> 00000000
  P1DIR |= BIT2 + BIT4;//0xFF-BIT5-BIT1-BIT6;//RoueA_BIT + RoueB_BIT + BIT3; 00000010100
  P1SEL |= BIT2 + BIT4;//0xFF-BIT5-BIT1-BIT6;// RoueA_BIT + RoueB_BIT + BIT3;
  P1SEL2 |= BIT4;
  //TACCR0 = 64;
  TACCTL0 = CCIE;
  TACCR0 = 1000;//65536-1; //32us = 31.25kHz
  TACCTL1 |= OUTMOD_7;
  TACCR1 = 0;//consigne_RoueA/2*1024;
  TACCTL2 |= OUTMOD_7;
  TACCR2 = 0;//*1024//
  TACTL |= TASSEL_2 | MC_1 | ID_0;                  // ACLK, upmode 
}

// --------------------------
// Fonction : 
// Description : 
// Param(s) : 
// Output : 
// --------------------------
void initInterrupt(void)
{
  /*P2DIR &= ~(BIT3 + BIT4); // optos
  P2IES |= (BIT3 + BIT4);     
  P2IFG &= ~(BIT3 + BIT4);    
  P2IE |= (BIT3+BIT4); */     
   __enable_interrupt();
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
