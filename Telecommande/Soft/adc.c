// adc.c


// Includes
#include "adc.h"

#include <io430x16x.h>


// Definitions
// --------------------------
// Fonction : adc_init
// Description : Fonction d'initialisation de l'ADC
// Param(s) : None
// Output : Nothing
// --------------------------
void adc_init(void)
{
  /* ADC12 Control Register 0 */
  // ADC12 sample-and-hold time -> 256 x ADC12CLKs,
  // Referance = 2.5V, Select ADC12MEM0
  ADC12CTL0 = SHT02 + ADC12ON+REFON;

  // Sample-and-hold source select -> ADC12SC bit, no clock divider,
  // clock -> ADC12OSC - internal osillator ~5MHz, Single-channel and single-conversion
  // ADC12CTL1 = SHP;

  // Sample-and-hold source select -> ADC12SC bit, no clock divider,
  // clock -> MCLK - external osillator ~2MHz, Single-channel and single-conversion
  // ADC12CTL1 = ADC12SSEL1+SHP;
  ADC12CTL1 = ADC12SSEL1;

  // Conversion Memory Control Registers
  // Select reference -> VR+ = AVCC+ and VR– = AVSS
  ADC12MCTL0 = 0x0;
}

// --------------------------
// Fonction : adc_read_ADC0
// Description : Fonction permettant la lecture de la valeur ADC convertie
// Param(s) : None
// Output : uint8_t, la valeur convertie
// --------------------------
uint16_t adc_read_ADC0(void)
{
  uint16_t val = 0;              // measured value

  ADC12MCTL0 = 0;               // clear
  ADC12MCTL0 |= INCH_0;         // Input channel select -> A0
  ADC12CTL0 |= (ADC12SC + ENC); // Enable and start conversion

  ADC12CTL0 &= ~ADC12SC;
  
  while (ADC12CTL1 & ADC12BUSY);// wait end of conversion  
  ADC12IFG = 0;                 // Clear interrupt flag
  
  val = ADC12MEM0;              // get ADC value

  ADC12CTL0 &= ~(ADC12SC + ENC);// Disable and stop conversion

  return val;                   // return value
}

// --------------------------
// Fonction : adc_read_ADC1
// Description : Fonction permettant la lecture de la valeur ADC convertie
// Param(s) : None
// Output : uint8_t, la valeur convertie
// --------------------------
uint16_t adc_read_ADC1(void)
{
  uint16_t val  = 0;                     // measured value

  
  ADC12MCTL0 = 0;                       // clear
  ADC12MCTL0 |= INCH_1;                 // Input channel select -> A1
  ADC12CTL0 |= (ADC12SC + ENC);         // Enable and start conversion  
  
  ADC12CTL0 &= ~ADC12SC;
  
  while (ADC12CTL1 & ADC12BUSY);        // wait end of conversion
  ADC12IFG = 0;                         // Clear interrupt flag  
    
  val = ADC12MEM0;                      // get ADC value
  
  ADC12CTL0 &= ~(ADC12SC + ENC);        // Disable and stop conversion

  return val;                           // return value
}

// --------------------------
// Fonction : adc_read_ADC2
// Description : Fonction permettant la lecture de la valeur ADC convertie
// Param(s) : None
// Output : uint8_t, la valeur convertie
// --------------------------
uint16_t adc_read_ADC2(void)
{
  uint16_t val  = 0;                     // measured value

  ADC12MCTL0 = 0;                       // clear
  ADC12MCTL0 |= INCH_2;                 // Input channel select -> A2

  ADC12CTL0 |= (ADC12SC + ENC);         // Enable and start conversion
  
  ADC12CTL0 &= ~ADC12SC;
  
  while (ADC12CTL1 & ADC12BUSY);        // wait end of conversion
  ADC12IFG = 0;                         // Clear interrupt flag
    
  val = ADC12MEM0;                      // get ADC value
  
  ADC12CTL0 &= ~(ADC12SC + ENC);        // Disable and stop conversion

  return val;                           // return value
}
