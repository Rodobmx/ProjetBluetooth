//
// Filename : led.c
// Author   : KevinM
// Modified : 05/04/2012
//


// Includes
#include "led.h"
#include <io430g2553.h>


// Defines
#define LEDR    BIT0
#define LEDG    BIT6


// Definitions
// ------------------------------------
// Fonction    : led_setup
// Description : Permet d'initialiser les leds
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_setup(void)
{
   P1DIR |= LEDR | LEDG;        // bits 0 et 6 du port 1 sont des sorties
   
   led_red_off();               // Extinction des leds
   led_green_off();             // ...
}

// ------------------------------------
// Fonction    : led_green_on
// Description : Permet d'allumer la led verte
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_green_on(void)
{
  P1OUT |= LEDG;                // Mise à 1 du bit 6 du port 1
                                //    xxxx xxxx
                                // OR  1
                                //     1
}

// ------------------------------------
// Fonction    : led_green_off
// Description : Permet d'éteindre la led verte
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_green_off(void)
{
  P1OUT &= ~LEDG;                // Mise à 0 du bit 6 du port 1
                                //    xxxx xxxx
                                // TODO
                                //
}

// ------------------------------------
// Fonction    : led_green_toggle
// Description : Permet d'inverser l'état de  la led verte
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_green_toggle(void)
{
  P1OUT ^= LEDG;                //     xxxx xxxx
                                // XOR  1
                                //     /x
}

// ------------------------------------
// Fonction    : led_red_on
// Description : Permet d'allumer la led rouge
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_red_on(void)
{
  P1OUT |= LEDR;                // Mise à 1 du bit 0 du port 1
                                //    xxxx xxxx
                                // OR         1
                                //            1
}

// ------------------------------------
// Fonction    : led_red_off
// Description : Permet d'éteindre la led rouge
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_red_off( void )
{
  P1OUT &= ~LEDR;               // Mise à 0 du bit 0 du port 1
                                //    xxxx xxxx
                                // TODO
                                //
}

// ------------------------------------
// Fonction    : led_red_toggle
// Description : Permet d'inverser l'état de  la led rouge
// Param(s)    : None
// Output      : Nothing
// ------------------------------------
void led_red_toggle( void )
{
  P1OUT ^= LEDR;                 //     xxxx xxxx
                                // XOR          1
                                //             /x
}
