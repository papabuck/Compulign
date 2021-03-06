// UTFT_ViewFont (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the included fonts.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <SPI.h>

// Declare which fonts we will be using
// extern uint8_t SmallFont[];
//extern uint8_t Ubuntu[];
//extern uint8_t DejaVuSans24[];
extern uint8_t Inconsola[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];


UTFT myGLCD;

void setup()
{
  myGLCD.InitLCD();

  myGLCD.clrScr();
}

void loop()
{
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);

  myGLCD.setFont(BigFont);
 /*
  myGLCD.print(" !\"#$%&'()*+,-./", CENTER, 0);
  myGLCD.print("0123456789:;<=>?", CENTER, 16);
  myGLCD.print("@ABCDEFGHIJKLMNO", CENTER, 32);
  myGLCD.print("PQRSTUVWXYZ[\\]^_", CENTER, 48);
*/
  myGLCD.print("0123456789:;<=>?", CENTER, 0);
  myGLCD.print("@ABCDEFGHIJKLMNO", CENTER, 16);
 /*
  myGLCD.print("`abcdefghijklmno", CENTER, 64);
  myGLCD.print("pqrstuvwxyz{|}~ ", CENTER, 80);

  myGLCD.setFont(SmallFont);
  
  myGLCD.print(" !\"#$%&'()*+,-./0123456789:;<=>?", CENTER, 120);
  myGLCD.print("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", CENTER, 132);
  myGLCD.print("`abcdefghijklmnopqrstuvwxyz{|}~ ", CENTER, 144);

  myGLCD.setFont(Ubuntu); 
  myGLCD.print("0123456789", CENTER, 0);
  myGLCD.print("ABCDEFGHIJ", CENTER, 40);

    myGLCD.setFont(DejaVuSans24); 
  myGLCD.print("0123456789", CENTER, 100);
  myGLCD.print("ABCDEFGHIJ", CENTER, 140);
*/
  myGLCD.setFont(Inconsola); 
  myGLCD.print("0123456789", CENTER, 40);
  myGLCD.print("ABCDEFGHIJ", CENTER, 80);

 myGLCD.setFont(SevenSegNumFont);
 myGLCD.print("0123456789", CENTER, 190);

  while(1) {};
}


