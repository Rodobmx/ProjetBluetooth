// 
// Filename : motor.c
// Author   : KevinM
// Modified : 05/04/2012
//


// Includes
#include "motor.h"
#include <io430g2553.h>


// Definitions
// --------------------------
// Fonction :
// Description :
// Param(s) :
// Output :
// --------------------------
void motor_setup(void)
{
  // 0 dec +> 00000000
  
  
  //TACCTL0 = CCIE;
  TA1CCTL0  = CCIE;
  TA1CCR0   = 1000;                      // 65536-1; //32us = 31.25kHz
  TA1CCTL1 |= OUTMOD_7;
  TA1CCR1   = 0;                         // consigne_RoueA/2*1024;
  TA1CCTL2 |= OUTMOD_7;
  TA1CCR2   = 0;                         // *1024
  TA1CTL   |= TASSEL_2 | MC_1 | ID_3;    // S%CLK, upmode
  TA1CTL &= ~TAIE;
  
  P2DIR  |= (BIT2 + BIT4 + BIT1 + BIT5);
  P2SEL  |= (BIT2 + BIT4);
  
  P2OUT  &= ~(BIT2 + BIT4);
  
  //TA1CTL = MC0;
  __enable_interrupt();
}

// --------------------------
// Fonction    : motor_setDir
// Description : ...
// Param(s)    : ...
// Output      : ...
// --------------------------
void motor_setDir(char l_wheel, char r_wheel)
{
  if(l_wheel == '+') P2OUT &= ~BIT1 ; else P2OUT|= BIT1;
  if(r_wheel == '+') P2OUT |= BIT5; else P2OUT &= ~BIT5;
}

// --------------------------
// Fonction    : motor_setSpd
// Description : ...
// Param(s)    : ...
// Output      : ...
// --------------------------
void motor_setSpd(uint8_t l_wheel, uint8_t r_wheel)
{
  TA1CCR1 = l_wheel*10;
  TA1CCR2 = r_wheel*10;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
  1==1;
}
