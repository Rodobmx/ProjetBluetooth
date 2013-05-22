// main_tlcmdBltth.c


// Includes
#include "tlcmdBltth.h"

#include "system.h"
#include "uart.h"
#include "led.h"
#include "jstck.h"
#include "lcd.h"
#include "adc.h"


// Main
int main( void )
{
  // init
  board_init();
  uart_setup(96);
  led_init();
  jstck_init();
  lcd_init();
  lcd_setContrast(0x45);
  adc_init();
  
  tlcmdBltth_init();
  
  // go
  tlcmdBltth_go();

  return 0;
}
