#include <Wire.h>
void WriteCommand(unsigned char ins)
{
  Wire.beginTransmission(0x78 >> 1);//0x78 >> 1
  Wire.write(0x00);//0x00
  Wire.write(ins);
  Wire.endTransmission();
}
void WriteData(unsigned char dat)
{
  Wire.beginTransmission(0x78 >> 1);//0x78 >> 1
  Wire.write(0x40);//0x40
  Wire.write(dat);
  Wire.endTransmission();
}
void IIC_SetPos(unsigned char x, unsigned char y)
{
  WriteCommand(0xb0 + y);
  WriteCommand(((x & 0xf0) >> 4) | 0x10); //|0x10
  WriteCommand((x & 0x0f) | 0x01); //|0x01
}
//全屏显示 -- Fill_Screen(0x00,0x00)可用作清屏
void Fill_Screen(unsigned char dat1, unsigned char dat2)
{
  unsigned char x, y;

  WriteCommand(0x02);    /*set lower column address*/
  WriteCommand(0x10);    /*set higher column address*/
  WriteCommand(0xB0);    /*set page address*/
  for (y = 0; y < 8; y++)
  {
    WriteCommand(0xB0 + y);  /*set page address*/
    WriteCommand(0x02);    /*set lower column address*/
    WriteCommand(0x10);    /*set higher column address*/

    for (x = 0; x < 64; x++)
    {
      WriteData(dat1);
      WriteData(dat2);
    }

  }
}
void initial() {
  WriteCommand(0xAE);    /*display off*/

  WriteCommand(0x02);    /*set lower column address*/
  WriteCommand(0x10);    /*set higher column address*/

  WriteCommand(0x40);    /*set display start line*/

  WriteCommand(0xB0);    /*set page address*/

  WriteCommand(0x81);    /*contract control*/
  WriteCommand(0x80);    /*128*/

  WriteCommand(0xA1);    /*set segment remap*/

  WriteCommand(0xA6);    /*normal / reverse*/

  WriteCommand(0xA8);    /*multiplex ratio*/
  WriteCommand(0x3F);    /*duty = 1/32*/

  WriteCommand(0xad);    /*set charge pump enable*/
  WriteCommand(0x8b);     /*    0x8a    外供VCC   */

  WriteCommand(0x30);    /*0X30---0X33  set VPP   9V 电压可以调整亮度!!!!*/

  WriteCommand(0xC8);    /*Com scan direction*/

  WriteCommand(0xD3);    /*set display offset*/
  WriteCommand(0x00);   /*   0x20  */

  WriteCommand(0xD5);    /*set osc division*/
  WriteCommand(0x80);

  WriteCommand(0xD9);    /*set pre-charge period*/
  WriteCommand(0x1f);    /*0x22*/

  WriteCommand(0xDA);    /*set COM pins*/
  WriteCommand(0x12);//0x02 -- duanhang xianshi,0x12 -- lianxuhang xianshi!!!!!!!!!

  WriteCommand(0xdb);    /*set vcomh*/
  WriteCommand(0x40);


  WriteCommand(0xAF);    /*display ON*/
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  initial();
  Fill_Screen(0x00, 0x00);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //IIC_SetPos(10,10);
  int i, j, k;
  for(i=0;i<128;i+=10)
  {
  for (j = 0; j < 8; j++) {
      IIC_SetPos(i, j);
      WriteData(0xFF);
      WriteData(0xFF);
      WriteData(0xFF);
      WriteData(0xFF);
      WriteData(0xFF);WriteData(0xFF);
    delay(16);
  }
    for (j = 7; j >= 0; j--) {
      IIC_SetPos(i+5, j);
      WriteData(0xFF);
      WriteData(0xFF);
      WriteData(0xFF);
      WriteData(0xFF);
      WriteData(0xFF);WriteData(0xFF);
    delay(16);
  }
  }

  delay(500);
  
  for(i=0;i<128;i+=10)
  {
  for (j = 0; j < 8; j++) {
      IIC_SetPos(i, j);
      WriteData(0);
      WriteData(0);
      WriteData(0);
      WriteData(0);
      WriteData(0);WriteData(0);
    delay(16);
  }
    for (j = 7; j >= 0; j--) {
      IIC_SetPos(i+5, j);
      WriteData(0);
      WriteData(0);
      WriteData(0);
      WriteData(0);
      WriteData(0);WriteData(0);
    delay(16);
  }
  }
  delay(500);
}
