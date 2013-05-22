// tlcmdBltth.c


// Includes
#include "tlcmdBltth.h"

#include "jstck.h"
#include "lcd.h"
#include "uart.h"
#include "utils.h"
#include "adc.h"

#include <io430x16x.h>
#include <string.h>


// Defines


// Variables
uint8_t screen;                         // pour changer d'ecran


// Definitions
// --------------------------
//
// --------------------------
void tlcmdBltth_init(void)
{
  screen = 1;
  
  lcd_clear();
  
/*
  uart_puts("$$$\r");
  uart_puts("SM,3\r");
  uart_puts("SR,00066648DD2E\r");
  uart_puts("---\r");
  */

  
}

// --------------------------
//
// --------------------------
void tlcmdBltth_go(void)
{
  uint16_t acc_ADC[3];          // ADC values ; 0:X, 1:Y, 2:Z
  //float    acc_f[3];            //
  uint8_t   v_X, v_Y;
  char     acc_s[3][5];        //
  
 
  while(1)
  {
    while(screen == 1)                  // start ?
    {
      jstck_clearDir();
      lcd_puts(0, 4, "START ?");
      lcd_update();
  
      while(jstck_getDir() != j_center);
        screen++;
    }
    
    while(screen == 2)
    {
      lcd_update();
      
      /*// TEST : itoa -> OK
      acc_ADC[0] = -123;
      itoa(acc_s[0], acc_ADC[0], 5, 10);
      lcd_puts(2, 0, "X : "); lcd_puts(2, 4, acc_s[0]);
      */
      
      /*// TEST : ftoa -> NOK
      acc_f[0] = 123.456f;
      ftoa(acc_s[0], acc_f[0]);
      lcd_puts(2, 0, "X : "); lcd_puts(2, 4, acc_s[0]);
      */
      
      
      // lecture des valeurs numériques des 3 axes
      acc_ADC[0] = adc_read_ADC0();   // X
      acc_ADC[1] = adc_read_ADC1();   // Y
      acc_ADC[2] = adc_read_ADC2();   // Z
      
      /*
      // Affichage des valeurs numériques
      itoa(acc_s[0], acc_ADC[0], 4, 10);
      itoa(acc_s[1], acc_ADC[1], 4, 10);
      itoa(acc_s[2], acc_ADC[2], 4, 10);
      
      // Affichage des valeurs
      lcd_puts(2, 0, "X : "); lcd_puts(2, 4, acc_s[0]);
      lcd_puts(3, 0, "Y : "); lcd_puts(3, 4, acc_s[1]);
      lcd_puts(4, 0, "Z : "); lcd_puts(4, 4, acc_s[2]);
      */
      
      
      
      /*// NOTES :
      //       -1       0      +1               (g)
      //|-------|-------|-------|---------------->  Acc
      // 
      // 0     0.85    1.65    2.45       3.3   (V)
      // |------|-------|-------|----------|----->  ANA
      //                  
      //       1055    2048    3040       4095
      // |------|-------|-------|----------|----->  ADC
      //
      //       25      50      74         100   (%)
      // |------|-------|-------|----------|----->
      
      
      // "Puisssance" | "Direction"
      //     +        |
      //     ^        |
      //     |        |     Beta
      //     + Alpha  | - <--+--> +
      //     |        |
      //     v        |
      //     -        |
      // 
      // Beta = 0  : l_dir = '+'
      //             r_dir = '+'
      //
      // Beta > 0  : l_dir = '+'
      //             r_dir = '-'
      //
      // Beta < 0  : l_dir = '-'
      //             r_dir = '+'
      //
      //
      // Alpha = 0 : l_spd = 0
      //             r_spd = 0
      //
      // Alpha > 0 : l_spd = f(Alpha)
      //             r_spd = f(Alpha)
      //
      // Alpha < 0 : l_spd = f(Alpha) ; ! il faut changer le sens des moteurs
      //             r_spd = f(Alpha) ; ! il faut changer le sens des moteurs
      //
      */
      
      char l_dir, r_dir;
      uint8_t l_spd, r_spd;
      
      if( acc_ADC[0] > 1950 &&  acc_ADC[0] < 2100)
      {
        l_spd = 0;
        r_spd = 0;
        l_dir = '+';
        r_dir = '+';
      }
      else if( acc_ADC[0] > 2100 && acc_ADC[0] < 2250)
      {
        v_X = (uint8_t)( ( (double)acc_ADC[0]/1.5) - 2100);
        l_spd = v_X;
        r_spd = v_X;
        l_dir = '+';
        r_dir = '+';
      }
      else if ( acc_ADC[0] > 2250 )
      {
        l_spd = 100;
        r_spd = 100;
        l_dir = '+';
        r_dir = '+';
      }
      else if( acc_ADC[0] < 1950 && acc_ADC[0] > 1800)
      {
        v_X = (uint8_t)(((double)acc_ADC[0]/1.5) - 1800);
        l_spd = v_X;
        r_spd = v_X;
        l_dir = '-';
        r_dir = '-';
      }
      else if ( acc_ADC[0] < 1800 )
      {
        l_spd = 100;
        r_spd = 100;
        l_dir = '-';
        r_dir = '-';
      }
      else if ( acc_ADC[1] < 2000 &&  acc_ADC[1] > 1850 )
      {
        v_Y = (uint8_t)(((double)acc_ADC[1]/1.5) - 1850);
        l_spd = 0;
        r_spd = v_Y;
        r_dir = '+';
        l_dir = '-';
      }
      else if ( acc_ADC[1] < 1850 )
      {
        l_spd = 0;
        r_spd = 100;
        r_dir = '+';
        l_dir = '-';
      }
      else if ( acc_ADC[1] > 2100 &&  acc_ADC[1] < 2270 )
      {
        v_Y = (uint8_t)(((double)acc_ADC[1]/1.7) - 2100);
        r_spd = 0;
        l_spd = v_Y;
        l_dir = '+';
        r_dir = '-';
      }
      else if ( acc_ADC[1] > 2270 )
      {
        r_spd = 0;
        l_spd = 100;
        l_dir = '+';
        r_dir = '-';
      }
      
      // Trame de vitesse à envoyer
      sendCmd(l_dir, l_spd, r_dir, r_spd);
      
      tempo_ms(200);

    }
  }
}

// --------------------------
// Fonction : sendCmd
// Description : Concatene les informations et les transmet en bluetooth
// Param(s) : char l_dir,    ['+' | '-']
//            uint8_t l_spd, [0 - 100]
//            char r_dir,    ['+' | '-']
//            uint8_t r_spd, [0 - 100]
//
// Output : Nothing
// --------------------------
void sendCmd(char l_dir, uint8_t l_spd, char r_dir, uint8_t r_spd)
{
  // 0123456789
  //           0123
  // -v:sggg,sddd; : envoie de vitesse
  // -v:+080,-080;
  
  char spd_s[4] = "000";
  char str_to_send[14];
  
  memset(str_to_send, ' ', 13);
  
  strcpy(str_to_send, "-v:");           // -v:
  str_to_send[3] = l_dir;               // s
  
  str_to_send[4] = '\0';
  
  itoa(spd_s, l_spd, 3, 10);            //
  strcat(str_to_send, spd_s);           // ggg
  
  str_to_send[7] = ',';                 // ,
  str_to_send[8] = r_dir;               // s
  
  str_to_send[9] = '\0';
  
  itoa(spd_s, r_spd, 3, 10);            //
  strcat(str_to_send, spd_s);           // ddd
  
  str_to_send[12] = '\0';
  
  strcat(str_to_send, ";");             // ;
  
  // Affichage des valeurs
  lcd_puts(2, 0, str_to_send);
  lcd_update();
  
  uart_puts(str_to_send);
  

      

  
}

// ------------------------------------
// Fonction permettant d'effectuer une tempo de x ms
// ------------------------------------
void tempo_ms(uint8_t ms)
{
  uint8_t i;
  for(i=0 ; i<ms; i++)
  {
    __delay_cycles(1000);
  }
}
