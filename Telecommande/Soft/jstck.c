// jstck.c


// Includes
#include "jstck.h"
#include <io430x16x.h>


// Variable
e_dir m_dir = j_none;      //uint8_t m_dir = ' ';


// Definitions
// --------------------------
//
// --------------------------
void jstck_init(void)
{
  
  P1DIR &= ~LEFT & ~DOWN & ~UP & ~RIGHT;
  P2DIR &= ~CENTER;
  
  P1SEL  &= ~LEFT & ~DOWN & ~UP & ~RIGHT;
  P2SEL  &= ~CENTER;
  
  P1IES &= ~LEFT & ~DOWN & ~UP & ~RIGHT;
  P2IES &= ~CENTER;
  
  P1IE |= LEFT | DOWN | UP | RIGHT;
  P2IE |= CENTER;
  
  __bis_SR_register(GIE);
}

// --------------------------
//
// --------------------------
e_dir jstck_getDir(void)
{
  while(m_dir == j_none);        //while(m_dir == ' ');
  return m_dir;
}

// --------------------------
//
// --------------------------
void jstck_clearDir(void)
{
  m_dir = j_none;               //m_dir = ' ';
}

// --------------------------
//
// --------------------------
void jstck_setDir(e_dir dir)
{
  m_dir = dir;
}

// --------------------------
//
// --------------------------
#pragma vector = PORT1_VECTOR
__interrupt void inter_jstck1(void)
{  
  if(P1IFG & UP )
    m_dir = j_up;
  else if(P1IFG & RIGHT)
    m_dir = j_right;
  else if(P1IFG & DOWN)
    m_dir = j_down;
  else if(P1IFG & LEFT)
    m_dir = j_left;
	
  P1IFG &= ~LEFT & ~DOWN & ~UP & ~RIGHT;      // On remet à 0 le registre P1IFG pour une future interruption
}

// --------------------------
//
// --------------------------
#pragma vector = PORT2_VECTOR
__interrupt void interr_jstck2(void)
{
  if((P2IFG & CENTER))
   m_dir = j_center;
	
  P2IFG &= ~CENTER;
}

