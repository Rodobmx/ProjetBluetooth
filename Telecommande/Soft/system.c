// system.c


/**********************************************************************************/
/*                                                                                */
/*    Copyright (C) 2005 OLIMEX  LTD.                                             */
/*                                                                                */
/*    Module Name    :  system module                                             */
/*    File   Name    :  system.c                                                  */
/*    Revision       :  01.00                                                     */
/*    Date           :  2006/01/26 initial version                                */
/*                                                                                */
/**********************************************************************************/

// Includes
#include <io430x16x.h>
#include "system.h"


// Definitions
/****************************************************************************/
/*  Init system frequency                                                   */
/*  Function : board_init                                                     */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/*  Description :                                                           */
/*  Tune DCOCLK around 800KHz and it is use for MCLK                        */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void board_init(void)
{
  // External oscilator 32kHz

  // Watchdog
  WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer

  // DCO Control Register
  // register:
  // MODx   4-0
  // DCOx   7-5
  // set:
  // MODx = 0, DCOx = 3
  // description:
  // modulation off, DCOx and RSELx ~800kHz
  DCOCTL = 0x60;

  // Basic Clock System Control Register 1
  // register:
  // RSELx  2-0
  // XT5V   3
  // DIVAx  5-4
  // XTS    6
  // XT20FF 7
  // set:
  // RSELx = 4, DIVAx = 4, XTS = 0, XT20FF = 1
  // description:
  // 1/4 for ACLK, set Lower mode, XT2 off
  BCSCTL1 = 0xA4;

  // Basic Clock System Control Register 2
  // register:
  // DCOR   0
  // DIVSx  1-2
  // SELS   3
  // DIVMx  4-5
  // SELMx  6-7
  // set:
  // DCOR = 0, DIVSx = 0, SELS = 0, DIVMx = 0, SELMx = 0
  // description:
  // internal resistor for DCO, no divider for MCLK and SCLK, MCLK and SCLK source -> DCOCLK
  BCSCTL2 = 0x00;
  

}

/****************************************************************************/
/*  Init system frequency                                                   */
/*  Function : board_init_32kHz                                             */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/*  Description :                                                           */
/*  LFXTCLK 32kHz is use for MCLK                                           */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void board_init_32kHz(void)
{
  // External oscilator 32kHz

  // Watchdog
  WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer

  // DCO Control Register
  // register:
  // MODx   4-0
  // DCOx   7-5
  // set:
  // MODx = 0, DCOx = 3
  // description:
  // modulation off, DCOx and RSELx ~800kHz
  DCOCTL = 0x60;

  // Basic Clock System Control Register 1
  // register:
  // RSELx  2-0
  // XT5V   3
  // DIVAx  5-4
  // XTS    6
  // XT20FF 7
  // set:
  // RSELx = 4, DIVAx = 0, XTS = 0, XT20FF = 1
  // description:
  // no divider for ACLK, set Lower mode, XT2 off
  BCSCTL1 = 0x84;

  // Basic Clock System Control Register 2
  // register:
  // DCOR   0
  // DIVSx  1-2
  // SELS   3
  // DIVMx  4-5
  // SELMx  6-7
  // set:
  // DCOR = 0, DIVSx = 0, SELS = 0, DIVMx = 0, SELMx = 0
  // description:
  // internal resistor for DCO, no divider for MCLK and SCLK, MCLK and SCLK source -> LFXTCLK (32kHz)
  BCSCTL2 = 0xC0;
}
