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
  P2DIR  |= BIT1 + BIT4;                // 0xFF-BIT5-BIT1-BIT6;//RoueA_BIT + RoueB_BIT + BIT3; 00000010100
  P2SEL  |= BIT1 + BIT4;                // 0xFF-BIT5-BIT1-BIT6;// RoueA_BIT + RoueB_BIT + BIT3;
  P2SEL2 &= ~(BIT1 + BIT4);
  
  //P2DIR |= BIT1 | BIT2;
  //P2DIR &= ~(BIT3 + BIT4);
  
  //TACCR0 = 64;
  //TA0CTL = MC_0 | TACLR;
  
  //TACCTL0 = CCIE;
  TA1CCTL0  = CCIE;
  TA1CCR0   = 1000;                      // 65536-1; //32us = 31.25kHz
  TA1CCTL1 |= OUTMOD_7;
  TA1CCR1   = 0;                         // consigne_RoueA/2*1024;
  TA1CCTL2 |= OUTMOD_7;
  TA1CCR2   = 0;                         // *1024
  TA1CTL   |= TASSEL_2 | MC_1 | ID_3;    // S%CLK, upmode
  TA1CTL &= ~TAIE;
  
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
  if(l_wheel == '+') P2OUT |= BIT5; else P2OUT &= ~BIT5;
  if(r_wheel == '+') P2OUT |= BIT2; else P2OUT &= ~BIT2;
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
