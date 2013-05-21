// led.c

// ATTENTION 
// la led est connectee sur le port P2.1 en mode cathode commune, donc
// pour allumer  la led, P2.1 = 0
// pour eteindre la led, P2.1 = 1
// pour reutiliser le code deja fait, j'ai simplement mis un 'toggle' dans chaque fct
//


// Includes
#include "led.h"

#include <io430.h>


// Defines
#define LEDG    BIT0
#define LEDR    BIT6


// Definitions
// ------------------------------------
// Fonction permettant d'initialiser les leds
// ------------------------------------
void led_init(void)
{
   P1DIR |= LEDG | LEDR;        // bits 0 et 7 du port 1 sont des sorties
   led_green_off();
   led_red_off();
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
  led_green_toggle();
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
  led_green_toggle();
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
// Fonction permettant d'allumer la led verte
// ------------------------------------
void led_red_on(void)
{
  P1OUT |= LEDR;                // Mise à 1 du bit 6 du port 1
                                //    xxxx xxxx
                                // OR  1
                                //     1
  led_red_toggle();
}

// ------------------------------------
// Fonction permettant d'éteindre la led verte
// ------------------------------------
void led_red_off(void)
{
  P1OUT &= ~LEDR;                // Mise à 0 du bit 6 du port 1
                                //    xxxx xxxx
                                // TODO
                                //
  led_red_toggle();
}

// ------------------------------------
// Fonction permettant d'inverser l'état de  la led verte
// ------------------------------------
void led_red_toggle(void)
{
  P1OUT ^= LEDR;                //     xxxx xxxx
                                // XOR  1
                                //     /x
}
