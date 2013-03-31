// main_bltth.c


// Includes
#include "system.h"
#include "uart.h"
#include "led.h"
#include "robot.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <io430x16x.h>


// Defines
#define ERR_NONE                        0
#define ERR_INCORRECT_START_TRAME       1
#define ERR_CMD_NON_RECONNUE            2


// Prototypes
//uint8_t atoi(char* str, uint8_t len);
uint8_t parse_cmd(char* querry);


// Main
int main( void )
{
  char querry[20];
  
  board_init();
  led_init();
  uart_init();
  
  
  while(1)
  {
    //uart_gets(querry, 20);              // TODO (Seb) : A remplacer par "uart_gets_until(querry, ';', 20)"
    uart_gets_until(querry, ';', 20);
    parse_cmd(querry);
    
    __delay_cycles(100);
  }
}


// Definitions
// --------------------------
// Fonction : 
// Description : 
// Param(s) : 
// Output : 
// --------------------------
/*uint8_t atoi(char* str, uint8_t len)
{

}
*/

// --------------------------
// Fonction : 
// Description : 
// Param(s) : 
// Output : 
// --------------------------
uint8_t parse_cmd(char* querry)
{
  if(querry[0] != '-')
    return ERR_INCORRECT_START_TRAME;
  else
  {
    switch(querry[1])
    {
      case 's':                                 // "-s;"
        TACCR1 = 0;
        TACCR2 = 0;
      break;
      
      case 'v':                                 // "-v:sggg,sddd;"
      {                                         //  0123456789012
        uint8_t l_wheel_dir = 0, r_wheel_dir = 0;
        uint8_t l_wheel_spd = 0, r_wheel_spd = 0;
        
        if(querry[3] == '-')
          l_wheel_dir = 1;
        if(querry[8] == '-')
          r_wheel_dir = 1;
        
        setSens(l_wheel_dir, r_wheel_dir);
        
        //l_wheel_spd = atoi(querry+4, 3);      // TODO (Seb) : creer une fonction "atoi(char* str, uint8_t len)"
        //r_wheel_spd = atoi(querry+9, 3);      // TODO (seb) : idem
        
        TACCR1 = l_wheel_spd*10;
        TACCR2 = r_wheel_spd*10;
      }
      break;
      
      case 'l':                                 // "-l:g,d;"
      {                                         //  0123456
        if(querry[3] == '1')
          led_red_toggle();
        if(querry[5] == '1')
          led_green_toggle();
      }
      break;
      
      default:
        return ERR_CMD_NON_RECONNUE;
      break;
    }
  }
  
  return ERR_NONE;
}
