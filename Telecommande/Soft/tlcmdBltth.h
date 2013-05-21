// tlcmdBltth.h

#ifndef _TLCMDBLTTH_H_
#define _TLCMDBLTTH_H_


// Includes
#include <stdint.h>


// Prototypes
void tlcmdBltth_init(void);
void tlcmdBltth_go(void);


void get_PitchAndRoll(double* pitch, double* roll, double ax, double ay, double az);

void sendCmd(char l_dir, uint8_t l_spd, char r_dir, uint8_t r_spd);


void tempo_ms(uint8_t ms);

#endif // _TLCMDBLTTH_H_
