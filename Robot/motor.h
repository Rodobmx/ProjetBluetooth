//
// Filename : bluetooth.h
// Author   : KevinM
// Modified : 05/04/2012
//

#ifndef _MOTOR_H_
#define _MOTOR_H_


// Includes
#include <stdint.h>


// Prototypes
void motor_setup(void);
void motor_setDir(char l_wheel, char r_wheel);
void motor_setSpd(uint8_t l_wheel, uint8_t r_wheel);

#endif
