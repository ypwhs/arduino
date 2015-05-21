#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

void setup() {
  Serial.begin(115200);
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
  refresh();
}
char a[25] = "Smart Parking";
char b[25] = "Powered.";
char c[25] = "By YPW.";
int i=0;
void refresh()
{
  u8g.firstPage();
do{
  //u8g.setFont(u8g_font_6x12);
  u8g.setFont(u8g_font_helvR12);
  u8g.drawStr( 0,13,a);
  u8g.drawStr( 0,13*2.4,b);
  u8g.drawStr( 0,13*4,c);
}
while( u8g.nextPage() );
}

int n=1;int MAX=2;
int d[10]={0};
long last=0, now=0;
void loop() {
  Serial.print("ask:[");
  Serial.print(n);
  Serial.println("]");
  char buffer[20] = {0};
  
  while(!Serial.available() && now-last<1000)now=millis();
  last=now;
  if(Serial.available())
  {
    Serial.readBytesUntil(10,buffer, 20);
    int cm=0,cm2=0,id=0,i=0;
    sscanf(buffer,"[%d]%d.%dcm",&id,&cm,&cm2);
    if(id==1)sprintf(a,"[%d]%d.%dcm",id,cm,cm2);
    else if(id==2)sprintf(b,"[%d]%d.%dcm",id,cm,cm2);
    //else sprintf(a,"[%d]%d.%dcm",id,cm,cm2);
    if(cm<7)d[id]=1;else d[id]=0;
    for(i=1;i<=MAX;i++)if(d[i]==0)break;
    if(i>MAX)sprintf(c, "no parking");
    else sprintf(c,"%d is OK.",i);
  }
  n++;if(n>MAX)n=1;
  refresh();
}
