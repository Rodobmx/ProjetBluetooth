// led.c
// Includes
#include <io430g2553.h>
#include "led.h"

// Defines
#define LEDR    BIT0
#define LEDG    BIT6


// Definitions
// ------------------------------------
// Fonction permettant d'initialiser les leds
// ------------------------------------
void led_init(void)
{
   P1DIR |= LEDR | LEDG;        // bits 0 et 7 du port 1 sont des sorties
   led_red_off();
   led_green_off();
}

// ------------------------------------
// Fonction permettant d'allumer la led verte
// ------------------------------------
void led_green_on(void)
{
  P1OUT |= LEDG;                // Mise à 1 du bit 6 du port 1
                                //    xxxx xxxx
                                // OR  1
                                //     1
}

// ------------------------------------
// Fonction permettant d'éteindre la led verte
// ------------------------------------
void led_green_off(void)
{
  P1OUT &= ~LEDG;                // Mise à 0 du bit 6 du port 1
                                //    xxxx xxxx
                                // TODO
                                //
}

// ------------------------------------
// Fonction permettant d'inverser l'état de  la led verte
// ------------------------------------
void led_green_toggle(void)
{
  P1OUT ^= LEDG;                //     xxxx xxxx
                                // XOR  1
                                //     /x
}

// ------------------------------------
// Fonction permettant d'allumer la led rouge
// ------------------------------------
void led_red_on(void)
{
  P1OUT |= LEDR;                // Mise à 1 du bit 0 du port 1
                                //    xxxx xxxx
                                // OR         1
                                //            1
}

// ------------------------------------
// Fonction permettant d'éteindre la led rouge
// ------------------------------------
void led_red_off(void)
{
  P1OUT &= ~LEDR;                // Mise à 0 du bit 0 du port 1
                                //    xxxx xxxx
                                // TODO
                                //
}

// ------------------------------------
// Fonction permettant d'inverser l'état de  la led rouge
// ------------------------------------
void led_red_toggle(void)
{
  P1OUT ^= LEDR;                 //     xxxx xxxx
                                // XOR          1
                                //             /x
}
