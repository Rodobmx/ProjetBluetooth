// main_bltth.c

// Includes
#include "system.h"
#include "uart.h"
#include "led.h"
#include "robot.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <io430g2553.h>


// Defines
#define ERR_NONE                        0
#define ERR_INCORRECT_START_TRAME       1
#define ERR_CMD_NON_RECONNUE            2


// Prototypes
//uint8_t atoi(char* str, uint8_t len);
//uint8_t parse_cmd(char* querry);


// Main
int main( void )
{
  //char querry[20];
  board_init();
  led_init();
  UART_Init();
  initInterrupt();
  //initTimer();
  
  //parseTrame();
  
  while(1)
  {
    //uart_gets(querry, 20);              // TODO (Seb) : A remplacer par "uart_gets_until(querry, ';', 20)"   
    GetTrame();
    //parseTrame();
    //UART_PutChar('a');
    //__delay_cycles(10000);
  }
}


