/***************************************************************************
* ���°汾�ġ�I2C_wtih_Wire���ᷢ���� http://www.14blog.com/archives/1358
* 
* ��������ⷴ�����뷢�ʼ��� hello14blog@gmail.com
***************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Wire.h"

#include "CN_SH1106_Wire.h"
#include "codetab.c"

//������
CN_SH1106_Wire::CN_SH1106_Wire(int rst)
{
  _rst = rst;
  pinMode(rst, OUTPUT);
}

//IICд���� -- ����������٣����ٶ�Ӱ�첻��
void CN_SH1106_Wire::WriteCommand(unsigned char ins)
{
  Wire.beginTransmission(0x78 >> 1);//0x78 >> 1
  Wire.write(0x00);//0x00
  Wire.write(ins);
  Wire.endTransmission();
}

//��ʼIICд���� -- ����������һ�����ݷ�����ɺ��ٹر�IIC���ܴܺ�̶�����
void CN_SH1106_Wire::WriteData(unsigned char dat)
{
  Wire.beginTransmission(0x78 >> 1);//0x78 >> 1
  Wire.write(0x40);//0x40
  Wire.write(dat);
  Wire.endTransmission();
}

//������ʼ������
void CN_SH1106_Wire::IIC_SetPos(unsigned char x, unsigned char y)
{
  WriteCommand(0xb0+y);
  WriteCommand(((x&0xf0)>>4)|0x10);//|0x10
  WriteCommand((x&0x0f)|0x01);//|0x01
}

//ȫ����ʾ -- Fill_Screen(0x00,0x00)����������
void CN_SH1106_Wire::Fill_Screen(unsigned char dat1,unsigned char dat2)
{
   unsigned char x,y;

     WriteCommand(0x02);    /*set lower column address*/       
     WriteCommand(0x10);    /*set higher column address*/
     WriteCommand(0xB0);    /*set page address*/
     for(y=0;y<8;y++)
        {
           WriteCommand(0xB0+y);    /*set page address*/
           WriteCommand(0x02);    /*set lower column address*/       
           WriteCommand(0x10);    /*set higher column address*/
           for(x=0;x<64;x++)
             {
              WriteData(dat1);
              WriteData(dat2);
             }
       }
}

//��ʾ16x16������
void CN_SH1106_Wire::ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	IIC_SetPos(x+2 , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteData(CN16x16[adder]);
		adder += 1;
	}
	IIC_SetPos(x+2,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteData(CN16x16[adder]);
		adder += 1;
	}
}

//SSD1106��ʼ��
void CN_SH1106_Wire::Initial()
{
//  digitalWrite(_rst, LOW);
  Wire.begin();
#if 0 //SH1106
  WriteCommand(0xAE);//display off
  
  WriteCommand(0x00);//set lower column address
  WriteCommand(0x10);//set higher column address
  
  WriteCommand(0x40);//set display start line
  
  WriteCommand(0xB0);//set page address
  
  WriteCommand(0x81);//�Աȶ�����
  WriteCommand(0xCF);//0~255���Աȶ�ֵ����Ч�������ر����ԣ�
  
  WriteCommand(0xA1);//set segment remap
  
  WriteCommand(0xA6);//normal / reverse
  
  WriteCommand(0xA8);//multiplex ratio
  WriteCommand(0x3F);//duty = 1/64
  
  WriteCommand(0xC8);//Com scan direction
  
  WriteCommand(0xD3);//set display offset
  WriteCommand(0x00);
  
  WriteCommand(0xD5);//set osc division
  WriteCommand(0x80);
  
  WriteCommand(0xD9);//set pre-charge period
  WriteCommand(0xF1);
  
  WriteCommand(0xDA);//set COM pins
  WriteCommand(0x12);
  
  WriteCommand(0xDB);//set vcomh
  WriteCommand(0x40);
  
  WriteCommand(0x8D);//set charge pump enable
  WriteCommand(0x14);
  
  WriteCommand(0xAF);//display ON
#endif
  
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
  WriteCommand(0x8b);     /*    0x8a    �⹩VCC   */

  WriteCommand(0x30);    /*0X30---0X33  set VPP   9V ��ѹ���Ե�������!!!!*/

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