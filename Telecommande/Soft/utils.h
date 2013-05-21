// utils.h

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

// Prototypes
void   itoa(char* dest, int16_t val, uint8_t digits, uint8_t base);      // OK
double radtoDeg(double rad);


void ftoa(char* p, float x);                                           // NOK

#endif
