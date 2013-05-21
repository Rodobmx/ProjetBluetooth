// lcd.h


// Includes
#include "lcd.h"

#include <stdlib.h>
#include <io430x16x.h>


// LCD memory index
unsigned int  LcdMemIdx;

// represent LCD matrix
unsigned char  LcdMemory[LCD_CACHE_SIZE];

// simple delay
void Delay(unsigned long a) { while (--a!=0); }


// Definitions
// --------------------------
// Function : lcd_init
// Description : Init LCD Controler
// Param(s) : Nothing
// Output  :  Nothing
// --------------------------
void lcd_init(void)
{
    //  Pull-up on reset pin.
  P5OUT_bit.P4 = 1;

  // Pin configuration - all as output
  P3DIR_bit.P0 = 1;  // STE0
  P3DIR_bit.P1 = 1;  // SIMO0
  P3DIR_bit.P2 = 1;  // SOMI0 (D/S)
  P3DIR_bit.P3 = 1;  // ULCK0
  P5DIR_bit.P4 = 1;  // RES

  // Pin select function
  P3SEL_bit.P0 = 0;  // STEO0
  P3SEL_bit.P1 = 1;  // SIMO0
  P3SEL_bit.P2 = 0;  // SOMI0 (D/S)
  P3SEL_bit.P3 = 1;  // ULCK


  //  Toggle display reset pin.
  P5OUT_bit.P4 = 0;
  Delay(10000);
  P5OUT_bit.P4 = 1;

  // Init SPI
  U0CTL   = 0x16;   // SPI Mode, 8bit, Master mode
  U0TCTL  = 0xB2;   // 3pin Mode, clock->SMCLK, no CKPL (poliarity), no CKPH (phase)
  //U0TCTL  = 0x30;   // 4pin Mode, clock->SMCLK, no CKPL (poliarity), no CKPH (phase)

  //U0BR0   = 0x2A;   // 19200 -> 19200 = ~800Khz/46
  //U0BR0   = 0x15;     // 38000 -> 38000 = ~800Khz/21
  U0BR0   = 0x02;
  U0BR1   = 0x00;
  UMCTL0  = 0x00;   // in spi mode don't used

  ME1     = 0x40;   // Enable SPI0
  ME2     = 0x01;   // Enable SPI0

  // Disable display controller.
  P3OUT_bit.P0 = 1;  // STE0

  Delay(100);

  // Send sequence of command
  lcd_send( 0x21, SEND_CMD );  // LCD Extended Commands.
  lcd_send( 0xC8, SEND_CMD );  // Set LCD Vop (Contrast).
  lcd_send( 0x06, SEND_CMD );  // Set Temp coefficent.
  lcd_send( 0x13, SEND_CMD );  // LCD bias mode 1:48.
  lcd_send( 0x20, SEND_CMD );  // LCD Standard Commands, Horizontal addressing mode.
  lcd_send( 0x08, SEND_CMD );  // LCD blank
  lcd_send( 0x0C, SEND_CMD );  // LCD in normal mode.
  
  // Clear and Update
  lcd_clear();
  lcd_update();
}

// --------------------------
// Function : lcd_send
// Description : Send to LCD
// Param(s) : data,   , l'information qui sera envoyée
//            cd,   [SEND_CHR or SEND_CMD], c'est une commande ou une donnée
// Output  :  Nothing
// --------------------------
void lcd_send(unsigned char data, unsigned char cd)
{
  // Enable display controller (active low).
  P3OUT_bit.P0 = 0;  // STE0 (CE)

  // command or data
  if(cd == SEND_CHR) {
    P3OUT_bit.P2 = 1;  // SOMI0 (D/S)
  }
  else {
    P3OUT_bit.P2 = 0;  // SOMI0 (D/S)
  }

  ///// SEND SPI /////

  //send data
  U0TXBUF = data;

  //Wait for ready U0TXBUF
  while((U0TCTL & TXEPT) == 0);

  // Disable display controller.
  P3OUT_bit.P0 = 1;  // STE0 (CE)
}

// --------------------------
// Function : lcd_update
// Description : Update LCD memory
// Param(s) : Nothing
// Output  :  Nothing
// --------------------------
void lcd_update(void)
{
  int i;

  //  Set base address X=0 Y=0
  lcd_send(0x80, SEND_CMD );
  lcd_send(0x40, SEND_CMD );

  //  Serialize the video buffer.
  for (i=0; i<LCD_CACHE_SIZE; i++)
  {
    lcd_send( LcdMemory[i], SEND_CHR );
  }
}

// --------------------------
// Function : lcd_clear
// Description : Clear LCD
// Param(s) : Nothing
// Output  :  Nothing
// --------------------------
void lcd_clear(void)
{
  int i;

  // loop all cashe array
  for (i=0; i<LCD_CACHE_SIZE; i++)
  {
     LcdMemory[i] = 0;
  }

}

// --------------------------
// Function : lcd_pixel
// Description : Change LCD Pixel mode
// Param(s) : x,    [], la position en x du pixel
//            y,    [], la position en y du pixel
//            mode,   , le mode 
// Output  :  Nothing
// --------------------------
void lcd_setPixel(unsigned char x, unsigned char y, unsigned char mode )
{
    unsigned int    index   = 0;
    unsigned char   offset  = 0;
    unsigned char   data    = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

    index = ((y / 8) * 84) + x;
    offset  = y - ((y / 8) * 8);

    data = LcdMemory[index];

    if ( mode == PIXEL_OFF )
    {
        data &= (~(0x01 << offset));
    }
    else if ( mode == PIXEL_ON )
    {
        data |= (0x01 << offset);
    }
    else if ( mode  == PIXEL_XOR )
    {
        data ^= (0x01 << offset);
    }
    LcdMemory[index] = data;
}

// --------------------------
// Function : lcd_getPixel
// Description : Recupère la valeur du pixel à la position x;y
// Param(s) : x, la position en x du pixel
//            y, la position en y du pixel
//            
// Output  :  la valeur du pixel
// --------------------------
unsigned char lcd_getPixel(unsigned char x, unsigned char y)
{
  unsigned int  index;
  unsigned char offset;
  unsigned char data;

  index  = ((y / 8) * 84) + x;
  offset = y - ((y / 8) * 8);
  
  data   = (LcdMemory[index] >> offset) & 0x01;
  
  return data;
}

// --------------------------
// Function : lcd_putc
// Description : Write a caracter on the screen, at the x y position
// Param(s) : x,  [], la position en x
//            y,  [], la position en y
//            ch,   , le caractere
// Output  :  Nothing
// --------------------------
void lcd_putc(unsigned char y, unsigned char x, unsigned char ch)
{
    unsigned int    index   = 0;
    unsigned int    i       = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

    index = (x*48 + y*48*14)/8 ;

    for ( i = 0; i < 5; i++ )
    {
      LcdMemory[index] = FontLookup[ch - 32][i] << 1;
      index++;
    }
}

// --------------------------
// Function : lcd_setContrast
// Description : Set the LCD contrast
// Param(s) : contrast, [], la valeur du contrast
// Output  :  Nothing
// --------------------------
void lcd_setContrast(unsigned char contrast)
{
    //  LCD Extended Commands.
    lcd_send( 0x21, SEND_CMD );

    // Set LCD Vop (Contrast).
    lcd_send( 0x80 | contrast, SEND_CMD );

    //  LCD Standard Commands, horizontal addressing mode.
    lcd_send( 0x20, SEND_CMD );
}

// --------------------------
// Function : lcd_puts
// Description : Send string to LCD
// Param(s) : row, [0-5],  la ligne de l'ecran
//            col, [0-13], la colonne à partir de laquelle sera inscrit le texte 
//            str,         la chaine de caracteres qui sera écrite
// Output  :  Nothing       
// --------------------------
void lcd_puts(unsigned char row, unsigned char col, char* str)
{
  // loop to the and of string
  while (*str)
  {
    lcd_putc(row, col,(*str));
    col++;
    str++;
  }
}

// --------------------------
// Function : lcd_drawFillRect
// Description : Draw a rectangle filled with black or white color.
// Param(s) : x0
//            y0
//            width
//	      height
//            color
// Output  :  Nothing       
// --------------------------
unsigned int lcd_drawFillRect(unsigned int x0, unsigned int y0,
                              unsigned int width, unsigned int height,
                              unsigned int color)
{
  unsigned int w,h;
  
  if( ( (x0+width)>LCD_X_RES)||( (y0+height)>LCD_Y_RES) ) return 0;
  
  for(w=0; w < width; w++)
    for(h=0; h < height; h++)
      lcd_setPixel(x0+w, y0+h, color);
  
  return 1;
}

// --------------------------
// Function : lcd_drawRect
// Description : Draw a rectangle filled with black or white color.
// Param(s) : x0
//            y0
//            width
//	      height
// Output  :  Nothing       
// --------------------------
unsigned int lcd_drawRect(unsigned int x0, unsigned int y0,
                          unsigned int width, unsigned int height)
{
  if( ( (x0+width)>LCD_X_RES)||( (y0+height)>LCD_Y_RES) ) return 0;

  lcd_drawHorizontalLine(x0, y0, width);        // Bord supérieur
  lcd_drawHorizontalLine(x0, y0+height, width); // Bord inférieur
  lcd_drawVerticalLine(x0, y0, height);         // Bord gauche
  lcd_drawVerticalLine(x0+width, y0, height);   // Bord droit
  
  return 1;
}

// --------------------------
// Function : lcd_drawVerticalLine
// Description : Draw a vertical line
// Param(s) : x0
//            y0
//            heigt
//
// Output  :  Nothing       
// --------------------------
void lcd_drawVerticalLine(unsigned int x0, unsigned int y0, unsigned int height)
{
  unsigned int i;
  
    for(i=0; i < height; i++)
      lcd_setPixel(x0, y0+i, PIXEL_ON);
}

// --------------------------
// Function : lcd_drawHorizontalLine
// Description : Draw a horizontal line
// Param(s) : x0
//            y0
//            width
//
// Output  :  Nothing       
// --------------------------
void lcd_drawHorizontalLine(unsigned int x0, unsigned int y0, unsigned int width)
{
  unsigned int i;
  
    for(i=0; i < width; i++)
      lcd_setPixel(x0+i, y0, PIXEL_ON);
}

// --------------------------
// Description : Draw a line
// Param(s) : x0, y0 : coordinates of the starting point
//            x1, y1 : coordinates of the destination point
//
// Output  :  Nothing       
// Source  : http://members.chello.at/~easyfilter/bresenham.html
// --------------------------
unsigned int lcd_drawLine(unsigned int x0, unsigned int y0,
                          unsigned int x1, unsigned int y1)
{
  if( (x0>LCD_X_RES)||(x1>LCD_X_RES) ) return 0;
  if( (y0>LCD_Y_RES)||(y1>LCD_Y_RES) ) return 0;
  

  int dx  =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy  = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = dx+dy, e2;                          /* error value e_xy */

  for(;;) 					/* loop */
  {
    lcd_setPixel(x0,y0, PIXEL_ON);

    if (x0==x1 && y0==y1) break;

    e2 = 2*err;
    if (e2 >= dy)				/* e_xy+e_x > 0 */
    {
      err += dy;
      x0  += sx;
    }

    if (e2 <= dx)				/* e_xy+e_y < 0 */
    {
      err += dx;
      y0  += sy;
    }
    
  }
  return 1;
}
