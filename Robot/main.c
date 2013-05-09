// 
// Filename : main.c


// Includes
#include "system.h"
#include "led.h"
#include "uart.h"
#include "bluetooth.h"
#include "motor.h"
#include <io430g2553.h>


// Variables
uint8_t running = 1;

// Main
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  // init
  board_setup();
  led_setup();
  bltth_setup();
  motor_setup();
  
  
  led_green_on();
  TA1CCR1   = 250;  
  TA1CCR2   = 250;  
  
  while(running)
  {
    running = bltth_parse();
    
  }
  while(1);
} 
