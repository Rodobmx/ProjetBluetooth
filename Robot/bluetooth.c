// 
// Filename : bluetooth.c
// Author   : KevinM
// Modified : 05/04/2012
//


// Includes
#include "bluetooth.h"
#include "motor.h"
#include "uart.h"
#include "led.h"

#include <string.h>             // memset
#include <stdlib.h>             // atoi
#include <io430g2553.h>


// Variables
e_bltthMode bltthMode;

char rx_buff[20];


// Definitions
// --------------------------
// Fonction    : bltth_setup
// Description : ...
// Param(s)    : ...
// Output      : ...
// --------------------------
void bltth_setup(void)
{
  uart_setup(96);
  bltthMode = BLTTH_WAITING_MODE;
  
  // Clear rx buffer
  memset(rx_buff, ' ', sizeof(rx_buff) );
  rx_buff[sizeof(rx_buff)-1] = '\0';
}

// --------------------------
// Fonction    : bllth_run
// Description : ...
// Param(s)    : ...
// Output      : ...
// --------------------------
/*
uint8_t bltth_run(void)
{
  uint8_t ret = 1;
  uint8_t nbChar;
  
  
  switch(bltthMode)
  {
    case BLTTH_CMD_MODE :
      // Nothing to do in this mode
      //uart_gets_until(rx_buff, sizeof(rx_buff), '\n');
      nbChar = uart_gets(rx_buff, 5);
      if(strcmp(rx_buff, "END")==0)
      {
        bltthMode = BLTTH_DATA_MODE;
        led_red_off();
        led_green_on();
      }
    break;
    
    case BLTTH_WAITING_MODE :
              
      led_red_on();
      led_green_on();
      
      //uart_gets_until(rx_buff, sizeof(rx_buff), '\n');
      nbChar = uart_gets_until(rx_buff, '\r', 5);
      if(strcmp(rx_buff, "CMD\r\n")==0)
      {
        bltthMode = BLTTH_CMD_MODE;
        led_red_on();
        led_green_off();
      }
      else
      {
          bltthMode = BLTTH_DATA_MODE;
          led_red_off();
          led_green_on();
      }
      
    case BLTTH_DATA_MODE :
      nbChar = uart_gets_until(rx_buff, '\r', 5);
//   
//      //uart_puts("\r\n");                                                
//      //uart_puts(">>> ");
//      
//      
//      memset(rx_buff, sizeof(rx_buff), ' ');
//      rx_buff[sizeof(rx_buff)-1] = '\0';
//                                                                        // ex :
//                                                                        // "-l:1,0;"
//      nbChar = uart_gets_until(rx_buff, sizeof(rx_buff)-1, ';');        //        ^
//      rx_buff[nbChar] = '\0';
//
//      if(rx_buff[0] == '-')                                             // "-l:1,0;"
//      {                                                                 //  ^
//        //uart_puts("\r\n");
//        //uart_puts("YOUR REQUEST : ");
//        //uart_puts(rx_buff);
//                                                                        //  012345678
//        switch(rx_buff[1])                                              // "-l:1,0;"
//        {                                                               //   ^
//          case 'h':
//            // TODO
//          break;
//          
//          case 'l':
//            if(rx_buff[3] == '1') led_red_on(); else led_red_off();
//            if(rx_buff[5] == '1') led_green_on(); else led_green_off();
//            
//          break;
//            
//          case 's':
//            //uart_puts("\r\n");
//            //uart_puts("-> S");
//          break;
//          
//          case 'v':
//            //uart_puts("\r\n");
//            //uart_puts("-> V");
//          break;
//          
//          case 'q':
//            //uart_puts("\r\n");
//            //uart_puts("-> Q");
//            ret = 0;
//            
//          break;
//          
//          default:
//            //uart_puts("\r\n");
//            //uart_puts("! commande non reconnue");
//          break;  
//        }
//      }
// 
    break;
  }
  return ret;
}
*/

// --------------------------
// Fonction    : bllth_run
// Description : ...
// Param(s)    : ...
// Output      : ...
// --------------------------
uint8_t bltth_parse(void)
{
  uint8_t ret = 1;
  
  if(uart_gets_startWithAndUntil(rx_buff, '-', '\n', 16) > 0)    // Si on a reçu des caracteres
  {
    switch(rx_buff[1])
    {
      case 'l':                                                         // ex :
      {                                                                 // "-l:1,0;"
        if(rx_buff[3] == '1') led_red_toggle();                         //  0123456789
        if(rx_buff[5] == '1') led_green_toggle();
      }
      break;
        
      case 's':                                                         // ex :
      {                                                                 // "-s;"
        //motor_setSpd(0,0);                                              
      }        
      break;
      
      case 'v':                                                         // ex :
      {                                                                 // "-v:*---,*---;"
        char    tmp_spd[4];                                             //  01234567890123456789
        char    l_wheel_dir = ' ', r_wheel_dir = ' ';
        uint8_t l_wheel_spd = 0, r_wheel_spd = 0;
        
        
        
        l_wheel_dir = (char)rx_buff[3];
        tmp_spd[0]  = rx_buff[4];
        tmp_spd[1]  = rx_buff[5];
        tmp_spd[2]  = rx_buff[6];
        tmp_spd[3]  = '\0';
          
        l_wheel_spd = atoi(tmp_spd);
        
        
        r_wheel_dir = (char)rx_buff[8];
        tmp_spd[0]  = rx_buff[9];
        tmp_spd[1]  = rx_buff[10];
        tmp_spd[2]  = rx_buff[11];
        tmp_spd[3]  = '\0';
        
        r_wheel_spd = atoi(tmp_spd);
        
        
        //motor_setDir(l_wheel_dir, r_wheel_dir);
        //motor_setSpd(l_wheel_spd, r_wheel_spd);
      }
      break;
      
      default:
      break;
    }
  }
  
  // Clear rx buffer
  memset(rx_buff, ' ', sizeof(rx_buff) );
  rx_buff[sizeof(rx_buff)-1] = '\0';
  
  return ret;
}
