// 
// Filename : main.c
// Author   : KevinM
// Modified : 05/04/2012
//


// Includes
#include "system.h"
#include "led.h"
#include "uart.h"
#include "bluetooth.h"
#include "motor.h"


// Variables
uint8_t running = 1;

// Main
int main(void)
{
  // init
  board_setup();
  led_setup();
  bltth_setup();
  motor_setup();
  
  
  // TESTS
  {
    //char strReceived[64];
    //char cReceived;
    //uint8_t nbCharReceived, nbCharSend;
    
    //uart_puts("WELCOME MASTER ! I WILL SERVE YOU .\r\n");
    /*
    // --------------------------------
    uart_puts("Sending a string.");
    uart_puts("\r\n");
    nbCharSend = uart_puts("TEST");                             // <-
    uart_puts("\r\n");
    uart_puts("x caracters send.");
    // --------------------------------
    
    // --------------------------------
    uart_puts("\r\n");
    uart_puts("Type 1 caracter.");
    uart_puts("\r\n");
    uart_puts(">>> ");
    cReceived = uart_getc();                                    // <-
    
    uart_puts("\r\n");
    uart_puts("you send : ");
    nbCharSend = uart_putc(cReceived);                          // <-
    uart_puts("\r\n");
    uart_puts("x caracters send.");
    // --------------------------------

    // --------------------------------
    uart_puts("\r\n");
    uart_puts("Type a string (length < 10).");
    uart_puts("\r\n");
    uart_puts(">>> ");
    nbCharReceived = uart_gets(strReceived, 10);                // <-
    uart_puts("\r\n");
    uart_puts("x caracters received.");
    uart_puts("\r\n");
    uart_puts("you send : ");
    uart_puts(strReceived);
    // --------------------------------

    // --------------------------------
    uart_puts("\r\n");
    uart_puts("Type a string end with '\\r' (uart_gets_until(...) ).");
    uart_puts("\r\n");
    uart_puts(">>> ");
    nbCharReceived = uart_gets_until(strReceived, '\r', 10);    // <-
    uart_puts("\r\n");
    uart_puts("x caracters received.");
    uart_puts("\r\n");
    uart_puts("you send : ");
    uart_puts(strReceived);
    // --------------------------------

    // --------------------------------
    uart_puts("\r\n");
    uart_puts("Type a string start with '-' and end with '\\r' (uart_gets_startWithAndUntil(...) ).");
    uart_puts("\r\n");
    uart_puts(">>> ");
    nbCharReceived = uart_gets_startWithAndUntil(strReceived, '-', '\r', 10);    // <-
    uart_puts("\r\n");
    uart_puts("x caracters received.");
    uart_puts("\r\n");
    uart_puts("you send : ");
    uart_puts(strReceived);
    // --------------------------------
    */
  }
  // FIN TEST
  
  while(running)
  {
    //running = bltth_run();
    running = bltth_parse();
  }
  
  //uart_puts("\r\nGOOD BYE ! SEE YOU LATER.");
  while(1);
} 
