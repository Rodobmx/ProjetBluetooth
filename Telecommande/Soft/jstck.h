// jstck.h

#ifndef _JSTCK_H_
#define _JSTCK_H_


// Defines
#define LEFT    BIT4
#define DOWN    BIT5
#define UP      BIT6
#define RIGHT   BIT7
#define CENTER  BIT0


// Enumeration
typedef enum e_dir{
  
  j_none   = ' ',
  j_up     = '^',
  j_right  = '>',
  j_down   = 'v',
  j_left   = '<',
  j_center = '+',
  
}e_dir;


// Prototypes
void  jstck_init(void);
e_dir jstck_getDir(void);
void  jstck_clearDir(void);
void  jstck_setDir(e_dir dir);

__interrupt void inter_jstck1(void);
__interrupt void interr_jstck2(void);

#endif
