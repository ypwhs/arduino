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
char c[25] = "Zhou Aide.";
char d[25] = "BTBU.";
char* text[] = {0,a,b,c,d};
int i=0;
void refresh()
{
  u8g.firstPage();
do{
  //u8g.setFont(u8g_font_6x12);
  u8g.setFont(u8g_font_helvR12);
  u8g.drawStr( 0,16,a);
  u8g.drawStr( 0,16*2,b);
  u8g.drawStr( 0,16*3,c);
  u8g.drawStr( 0,16*4,d);
}
while( u8g.nextPage() );
}

int n=1;int MAX=4;
int distance[10]={0};
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
    sprintf(text[id],"[%d]%d.%dcm",id,cm,cm2);
//    else sprintf(a,"[%d]%d.%dcm",id,cm,cm2);
//    if(cm<7)distance[id]=1;else distance[id]=0;
//    for(i=1;i<=MAX;i++)if(distance[i]==0)break;
//    if(i>MAX)sprintf(c, "no parking");
//    else sprintf(c,"%d is OK.",i);
  }
  n++;if(n>MAX)n=1;
  refresh();
}
