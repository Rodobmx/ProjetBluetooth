// tlcmdBltth.c


// Includes
#include "tlcmdBltth.h"

#include "../jstck.h"
#include "../lcd.h"
#include "../uart.h"
#include "../utils.h"
#include "../adc.h"

#include <io430x16x.h>
#include <string.h>
#include <math.h>


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
}

// --------------------------
//
// --------------------------
void tlcmdBltth_go(void)
{
  
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
      int16_t a_raw[3];             // ADC values ; 0:X, 1:Y, 2:Z
      double pitch = 0, roll = 0;   // roulis et tangage
      
      char l_dir, r_dir;            // Variables utilisées pour
      uint8_t l_spd, r_spd;         // la creation de la trame à envoyer

      
      //float    acc_f[3];            //
      //char     acc_s[3][5];         //
      
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
      */
      a_raw[0] = adc_read_ADC0();   // X
      a_raw[1] = adc_read_ADC1();   // Y
      a_raw[2] = adc_read_ADC2();   // Z
      
      /* // Affichage des valeurs numériques
      itoa(acc_s[0], acc_ADC[0], 4, 10);
      itoa(acc_s[1], acc_ADC[1], 4, 10);
      itoa(acc_s[2], acc_ADC[2], 4, 10);
      
      // Affichage des valeurs
      lcd_puts(2, 0, "X : "); lcd_puts(2, 4, acc_s[0]);
      lcd_puts(3, 0, "Y : "); lcd_puts(3, 4, acc_s[1]);
      lcd_puts(4, 0, "Z : "); lcd_puts(4, 4, acc_s[2]);
      */
      
      // TODO : raw to g
      get_PitchAndRoll(&pitch, &roll, a_raw[0], a_raw[1], a_raw[2]);
      
      pitch = radtoDeg(pitch);
      roll  = radtoDeg(roll);
      
      // TODO : en fonction des differents cas on change de sens les moteurs
      /* // NOTES :
      //     "Puisssance"  | "Direction"
      //   +               |
      //   ^               |
      //   |               | Beta (Roll)
      //   + Alpha (Pitch) | - <--+--> +
      //   |               |
      //   v               |
      //   -               |
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
      // TODO : changement d'echelle de pitch et roll, en degrées -> valeur variant entre 0 et 100
      l_dir = '+';
      l_spd = 80;       // f(pitch); pour avoir une valeur qui varie entre 0 et 100
      r_dir = '-';
      r_spd = 80;       // f(pitch);

      
      // Trame de vitesse à envoyer
      sendCmd(l_dir, l_spd, r_dir, r_spd);

      // Delay
      tempo_ms(200);
    }
  }
}

// --------------------------
// Fonction    : get_PitchAndRoll
// Description : ...
// Param(s)    : 
//               
//               
// Output : Nothing
// --------------------------
void get_PitchAndRoll(double* pitch, double* roll, double ax, double ay, double az)
{
  double magnitude = sqrt((ax * ax) + (ay * ay) + (az * az));
  
  ax = ax / magnitude;
  ay = ay / magnitude;
  az = az / magnitude;
  
  *pitch = -atan2(ax, sqrt(pow(ay,2) + pow(az, 2)));
  *roll  = atan2(ay, az);
  
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
  
  //uart_puts(str_to_send);
  
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
