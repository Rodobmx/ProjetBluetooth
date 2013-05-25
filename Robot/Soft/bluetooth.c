// 
// Filename : bluetooth.c
//


// Includes
#include "bluetooth.h"
#include "motor.h"
#include "uart.h"
#include "led.h"

#include <string.h>             // memset
#include <stdlib.h>             // atoi
#include <io430g2553.h>


#define TRAME_CAR_START '-'             // Caractere debut de trame
#define TRAME_CAR_SEPARCMD ':'          // caractere separation commande
#define TRAME_CAR_END ';'               // caractere fin de trame
#define TRAME_CMD_INDEX 1               // index du caractere de commande
#define TRAME_SENSA_INDEX 3             // index signe roue A
#define TRAME_SENSB_INDEX 8             // index signe roue B
#define TRAME_CAR_SEPAR ','
#define TRAME_CAR_SENS_POS '+'
#define TRAME_CAR_SENS_NEG '-'
#define TRAME_CAR_VITESSE 'v'            // -v:+100,+100; (
#define TRAME_CAR_LED 'l'               // -l:g,d; -l:0,0;  -l:1,1;
#define TRAME_LEDG_INDEX 3             // index led gauche
#define TRAME_LEDD_INDEX 5             // index led droite
#define TRAME_CAR_AU 's'                // Arret Urgence 
#define TRAME_MAX_SIZE  16


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
  //memset(rx_buff, ' ', sizeof(rx_buff) );
  //rx_buff[sizeof(rx_buff)-1] = '\0';
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
  char buffer[TRAME_MAX_SIZE];/* = "-v:+100,-100;"; test*/
  while(1)
  {
  if(uart_gets_startWithAndUntil(buffer, TRAME_CAR_START, TRAME_CAR_END, TRAME_MAX_SIZE) > 0)    // Si on a reçu des caracteres
  {
    switch(buffer[TRAME_CMD_INDEX])
    {
      case TRAME_CAR_VITESSE: // Type trame recue 2 vitesse des roues   
        {
          unsigned vitesseA=0, vitesseB=0;
          
          
          /* On peut réecrire la fonction atoi pour prendre que 3 caractere si tu veux, sinon comme sa c'est plus rapide que mettre en temp */
          vitesseA = (buffer[TRAME_SENSA_INDEX+1]-48)*100 +(buffer[TRAME_SENSA_INDEX+2]-48)*10 + (buffer[TRAME_SENSA_INDEX+3]-48);
          vitesseB = (buffer[TRAME_SENSB_INDEX+1]-48)*100 +(buffer[TRAME_SENSB_INDEX+2]-48)*10 + (buffer[TRAME_SENSB_INDEX+3]-48);
          
          motor_setDir(buffer[TRAME_SENSA_INDEX],buffer[TRAME_SENSB_INDEX]);     
          motor_setSpd(vitesseA,vitesseB);
          
        }      
        break;
   
      case TRAME_CAR_AU:
        TA1CCR1 = 0;
        TA1CCR2 = 0;
        break;
                  
      case TRAME_CAR_LED:
        if(buffer[TRAME_LEDG_INDEX] == '1')
          led_green_toggle();
        /*else
          led_green_off();
        */
        if(buffer[TRAME_LEDD_INDEX]== '1')
           led_red_toggle();
        /*else
          led_red_off();*/
        break;
                    
      default:
        break;
      
    } 
  }
  }
  /* Inutile, la lecture est une tache bloquante (attente d'un caractere), on ne peut donc pas revenir sur une trame recue.
  // Clear rx buffer
  memset(rx_buff, ' ', sizeof(rx_buff) );
  rx_buff[sizeof(rx_buff)-1] = '\0';
  */
  return ret;
}
