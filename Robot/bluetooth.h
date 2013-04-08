// 
// Filename : bluetooth.h
// Author   : KevinM
// Modified : 05/04/2012
//

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_


// Includes
#include <stdint.h>


// Enumerations
typedef enum e_bltthMode{

  BLTTH_WAITING_MODE,
  BLTTH_DATA_MODE,
  BLTTH_CMD_MODE
  //BLTTH_WAITING_FOR_DATA_MODE,
  //BLTTH_WAITING_FOR_CMD_MODE,

} e_bltthMode;


// Prototypes
void    bltth_setup(void);
uint8_t bltth_run(void);
uint8_t bltth_parse(void);

#endif
