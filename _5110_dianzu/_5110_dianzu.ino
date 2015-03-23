// LCD5110_Graph_Demo (C)2013 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of most of the functions
// in the library.
//
// This program requires a Nokia 5110 LCD module.
//
// It is assumed that the LCD module is connected to
// the following pins using a levelshifter to get the
// correct voltage to the module.
//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12
//
#include <LCD5110_Graph.h>

LCD5110 myGLCD(2, 3, 4, 6, 5);

extern uint8_t SmallFont[];
extern uint8_t arduino_logo[];
extern unsigned char TinyFont[];
extern uint8_t The_End[];
extern uint8_t pacman1[];
extern uint8_t pacman2[];
extern uint8_t pacman3[];
extern uint8_t pill[];

float y;
uint8_t* bm;
int pacy;

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  myGLCD.clrScr();
  myGLCD.drawBitmap(0, 0, arduino_logo, 84, 48);
  myGLCD.update();
  delay(500);
}
int i,j;
long s,last;
void loop()
{
  myGLCD.clrScr();
  for (i = 0; i < 84; i++)
  {  s=0;
    for(j=0;j<63;j++)s += analogRead(A0)/63;
    myGLCD.printNumI(s, 64, 40);
    s = s / 22;
    myGLCD.drawLine(i, last, i + 1, s);
    myGLCD.update();
//    delay(50);
    last = s;
  }
}
