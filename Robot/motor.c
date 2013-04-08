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
  P1DIR  |= BIT2 + BIT4;                // 0xFF-BIT5-BIT1-BIT6;//RoueA_BIT + RoueB_BIT + BIT3; 00000010100
  P1SEL  |= BIT2 + BIT4;                // 0xFF-BIT5-BIT1-BIT6;// RoueA_BIT + RoueB_BIT + BIT3;
  P1SEL2 |= BIT4;
  
  P2DIR |= BIT1 | BIT2;
  P2DIR &= ~(BIT3 + BIT4);
  
  //TACCR0 = 64;
  TACCTL0  = CCIE;
  TACCR0   = 1000;                      // 65536-1; //32us = 31.25kHz
  TACCTL1 |= OUTMOD_7;
  TACCR1   = 0;                         // consigne_RoueA/2*1024;
  TACCTL2 |= OUTMOD_7;
  TACCR2   = 0;                         // *1024
  TACTL   |= TASSEL_2 | MC_1 | ID_0;    // ACLK, upmode
}

// --------------------------
// Fonction    : motor_setDir
// Description : ...
// Param(s)    : ...
// Output      : ...
// --------------------------
void motor_setDir(char l_wheel, char r_wheel)
{
  if(l_wheel == '+') P2OUT |= BIT1; else P2OUT &= ~BIT1;
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
  TACCR1 = l_wheel*10;
  TACCR2 = r_wheel*10;
}
