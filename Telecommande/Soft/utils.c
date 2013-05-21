// utils.c


// Includes
#include "utils.h"

#include <math.h>
#include <string.h>


// NOTES
//
// 123 = 3 digits
// (int)(log(123)+1) = 3 (3 digits)
//


// Constantes
const double pi = 3.14159f; // pi = 4. * atan(1.);


// Definitions
// --------------------------
// Fonction     : itoa
// Description  : Converti un nombre ENTIER en une chaine de caractères
//
// Param(s)     : char* dest,     l'@ du tableau dans lequel sera stocké le nombre converti
//                uint16_t val,   le nombre à convertir
//                uint8_t digits, le nombre de chiffres que contient le nombre à convertir
//                uint8_t base,   la base dans laquelle sera convertie le nombre,            [2 | 10 | 16]
//
// Output       : Nothing
//
// Restrictions : - la taille du tableau de destination DOIT être suffisement grande
// --------------------------
void itoa(char* dest, int16_t val, uint8_t digits, uint8_t base)
{ 
  const char digitMap[] = "0123456789ABCDEF";
  
  // Checks
  if(base == 00 || base > 16) return;
  
  // Reset dest buffer
  memset(dest, '0', digits-1);
  dest[digits] = '\0';          // make sure the output string is terminated
  
  
  //
  if(val < 0)
  {
    dest[0] = '-';
    val *= -1;
  }
  
  do
  {
    dest[--digits] = digitMap[val%base];
    val/= base;
  }while(val > 0);
  
}

// --------------------------
// Fonction     : radtoDeg
// Description  : Converti un angle en radian en un angle en degrées
//
// Param(s)     : double rad, l'angle en radian à convertir
//
// Output       : double, l'angle en degrées
// --------------------------
double radtoDeg(double rad)
{
  return rad * 180 / pi; //radians to degree
}

// --------------------------
// Fonction    : ftoa
// Description : Converti un nombre ... en une chaine de caractères
//
// Param(s)    : char* p,   l'@ du tableau dans lequel sera stocké le nombre converti
//               float x,   le nombre à convertir
//
// Output      : Nothing
//
// Restrictions : ...
//                
// Sources : http://stackoverflow.com/questions/2268725/how-can-i-convert-a-float-double-to-ascii-without-using-sprintf-or-ftoa-in-c
// --------------------------
void ftoa(char* p, float x)
{
  int n, i=0, k=0;
  
  
  // Partie entière
  n =(int)x;
  while(n>0)
  {
    x/=10;
    n=(int)x;
    i++;
  }
  
  *(p+i) = '.';
  x *= 10;
  n = (int)x;
  x = x-n;
  
  while(n>0)
  {
    if(k == i)
      k++;
    *(p+k)=48+n;
    x *= 10;
    n = (int)x;
    x = x-n;
    k++;
  }
  // make sure the output string is terminated
  *(p+k) = '\0';
}
