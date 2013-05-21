// adc.h

#ifndef _ADC_H_
#define _ADC_H_


// Includes
#include <stdint.h>


// Prototypes
void adc_init(void);
uint16_t adc_read_ADC0(void);
uint16_t adc_read_ADC1(void);
uint16_t adc_read_ADC2(void);

#endif // _ADC_H_
