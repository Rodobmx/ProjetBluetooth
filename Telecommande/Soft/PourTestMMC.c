/**********************************************************************************/
/*                                                                                */
/*    Copyright (C) 2005 OLIMEX  LTD.                                             */
/*                                                                                */
/*    Module Name    :  main module                                               */
/*    File   Name    :  main.c                                                    */
/*    Revision       :  01.00                                                     */
/*    Date           :  2006/01/26 initial version                                */
/*                                                                                */
/**********************************************************************************/
#include "system.h"
#include "lcd_new.h"
//#include "mmc.h"
#include <io430x16x.h>
#include <string.h>

#define DELAY_1   0

#define L_BMP   18
#define H_BMP   16

static const unsigned char bitmap1[H_BMP][L_BMP]  = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,
  0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
  0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,0,0,0,
  0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,0,0,
  0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
  0,0,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,
  0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,0,
  0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,
  0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

unsigned char i = 0;
//int     X, Y;
int     X_ini, Y_ini;

/*
// mmc variable
extern char mmc_buffer[T_BUF];
char mmc_buffer_test_1[T_BUF];
char state_mmc = 1;
*/

// menu variable
unsigned char pos = 0;
char press_up   = 1;
char press_down = 1;
char press_joys = 1;

// simple delay
void DelayN(unsigned long a) { while (--a!=0); }
void Stop(void);

// Restart
void Restart(void) {

  typedef void (* Fpnt_t)(void);
  Fpnt_t Fpnt;
  Fpnt = (Fpnt_t)*(unsigned int *)0xFFFE;
  Fpnt();
}

void Move(int X, int Y)
{
  LCDPixel(X, Y, 1);
  LCDUpdate();
}

void sous_menu(int *nb_choix)
{
    *nb_choix=2;
    
    LCDClear();
    LCDStr ( 0, " Test graph." );
    LCDStr ( 1, " Test SD" );
    LCDStr ( pos-1, ">" );
    LCDUpdate();
}

// vizualize menu
void Menu(void)
{
  int nb_choix;
  
  sous_menu(&nb_choix);

  //if joystick was pressed
  while((P2IN&BIT0) == 0) {}

  // loop for choose
  while(P2IN&BIT0)
  {
    // Up
    if((!(P1IN&BIT6))&&(press_up==1))
    {
      pos--;
      if(pos<1)
        pos = 1;
      sous_menu(&nb_choix);
      press_up = 0;
    }
    if((P1IN&BIT6)&&(press_up==0))
    {
      press_up = 1;
    }

    // Down
    if((!(P1IN&BIT5))&&(press_down==1))
    {
      pos++;
      if(pos>nb_choix)
        pos = nb_choix;
      sous_menu(&nb_choix);
      press_down = 0;
    }
    if((P1IN&BIT5)&&(press_down==0))
    {
      press_down = 1;
    }
  }
}
void
test_graph()
{
    int i,j;

    // visualize
    LCDClear();
    for(i=0; i<L_BMP; i++)
      for(j=0; j<H_BMP; j++)
        LCDPixel(X_ini+i, Y_ini+j, bitmap1[j][i]);
    LCDStr ( 5, " back to menu " );
    LCDUpdate();
    // led - P2.1
    P2OUT &= ~BIT1;   // low
    P2DIR |= BIT1;    // output
}

/*
void
test_SD()
{  
    // CP and WP as input
    P5DIR_bit.P5 = 0;
    P5DIR_bit.P6 = 0;
  
    // card present
    while(P5IN&BIT6)
    {
      LCDClear();
      LCDStr ( 0, "Card not      " );
      LCDStr ( 1, "present!      " );
      LCDStr ( 3, "Please put it " );
      LCDStr ( 4, "and press to  " );
      LCDStr ( 5, "continue!     " );
      LCDUpdate();
    }
    while(P2IN&BIT0);
    // write protect
    while(P5IN&BIT5)
    {
      LCDClear();
      LCDStr ( 0, "Card is write " );
      LCDStr ( 1, "protect!      " );
    //  LCDStr ( 3, "Please correct" );
    //  LCDStr ( 4, "it and press  " );
    //  LCDStr ( 5, "to continue!  " );
      LCDUpdate();
    }
    while(P2IN&BIT0); 
    if (initMMC() == MMC_SUCCESS)	// card found
    {
      //card_state |= 1;
      memset(&mmc_buffer,0,T_BUF);
      mmcReadRegister (10, 16);
      mmc_buffer[7]=0;

      // Fill first Block (0) with 'A'
      memset(&mmc_buffer,'0',T_BUF);    //set breakpoint and trace mmc_buffer contents
      mmcWriteBlock(0);
      // Fill second Block (1)-AbsAddr T_BUF with 'B'
      memset(&mmc_buffer,'1',T_BUF);
      mmcWriteBlock(T_BUF);

      // Read first Block back to buffer
      memset(&mmc_buffer,0x00,T_BUF);
      mmcReadBlock(0,T_BUF);
      memset(&mmc_buffer_test_1,'0',T_BUF);
      if(strncmp(&mmc_buffer[0], &mmc_buffer_test_1[0], T_BUF))
        state_mmc=0;

      // Read first Block back to buffer
      memset(&mmc_buffer,0x00,T_BUF);
      mmcReadBlock(T_BUF,T_BUF);
      memset(&mmc_buffer_test_1,'1',T_BUF);
      if(strncmp(&mmc_buffer[0], &mmc_buffer_test_1[0], T_BUF))
        state_mmc=0;
      memset(&mmc_buffer,0x00,T_BUF);
    }
    LCDClear();
    LCDStr ( 0, "Test MMC card " );

    if(state_mmc == 1)
      LCDStr ( 1, "  SUCCESS!!!  " );
    if(state_mmc == 0)
      LCDStr ( 1, "  FAILED!!!   " );
    LCDUpdate();
    while(1)
    {
      if(((P2IN&BIT0) == 0)&&(press_joys == 1))
      {
        press_joys = 0;
        Restart();
      }
      if(((P2IN&BIT0) == 1)&&(press_joys == 0))
      {
        press_joys = 1;
      }
    }
}
*/

/****************************************************************************/
/*  Main function                                                           */
/*  Function : main                                                         */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
int main( void )
{
  /**** INITIALIZATION ****/
  pos = 1;
  X_ini=20;
  Y_ini=10;
  
  //all as input
  P1DIR = 0x0;
  P2DIR = 0x0;
  P3DIR = 0x0;
  P4DIR = 0x0;
  P5DIR = 0x0;
  P6DIR = 0x0;

  // Frequency
  InitFreq();

  // LCD init
  LCDInit();
  LCDContrast(0x45);

  // Show menu
  Menu();

  // choose Graphics test
  if(pos == 1)
  {
    // pressed joystick
    press_joys = 0;
    DelayN(1000);
    // if joystick was pressed
    while((P2IN&BIT0) == 0) {}
    test_graph();
  }
  // choose mmc card
  if(pos == 2)
  {
    // pressed joystick
    press_joys = 0;
    DelayN(1000);
    // if joystick was pressed
    while((P2IN&BIT0) == 0) {}
    //test_SD();
  }
   
  //gestion joystick
  while(1)
  {
    // left
    if(!(P1IN&BIT7))
    {
      //ajouter traitement "gauche" ici
    }
    // right
    else if(!(P1IN&BIT4))
    {
      //ajouter traitement "droite" ici
    }
    // Up
    else if(!(P1IN&BIT6))
    {
      //ajouter traitement "haut" ici
    }
    // Down
    else if(!(P1IN&BIT5))
    {
      //ajouter traitement "bas" ici
    }
    // press joystick
    if(((P2IN&BIT0) == 0)&&(press_joys == 1))
    {
       press_joys = 0;
       Restart();
    }
    if(((P2IN&BIT0) == 1)&&(press_joys == 0))
    {
       press_joys = 1;
    }
    LCDStr ( 5, " back to menu " );
    LCDUpdate();
  }
}
