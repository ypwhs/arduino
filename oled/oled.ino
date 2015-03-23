#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

void setup() {
  Serial.begin(9600);
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
char a[8] = "Powered";
char b[8] = "By YPW.";
int i=0;
void loop() {
  while(Serial.available())
  {
    if(i<7)a[i]=Serial.read();
    else if(i<14)b[i-7]=Serial.read();
    else {
      i=0;
      a[i]=Serial.read();
    }
    i++;
  }
u8g.firstPage();
do{
  //u8g.setFont(u8g_font_6x12);
  u8g.setFont(u8g_font_helvR24);
  u8g.drawStr( 0,26,a);
  u8g.drawStr( 0,52,b);
}
while( u8g.nextPage() );
}
