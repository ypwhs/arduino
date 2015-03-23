/***************************************************************************
* ���°汾�ġ�I2C_wtih_Wire���ᷢ���� http://www.14blog.com/archives/1358
* 
* ��������ⷴ�����뷢�ʼ��� hello14blog@gmail.com
***************************************************************************/

#ifndef _CN_SH1106_Wire_H
#define _CN_SH1106_Wire_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class CN_SH1106_Wire
{
 public:
  CN_SH1106_Wire(int rst);
  
  void WriteCommand(unsigned char ins);
  void WriteData(unsigned char dat);
  void IIC_SetPos(unsigned char x, unsigned char y);
  void Fill_Screen(unsigned char dat1,unsigned char dat2);
  void ShowCN(unsigned char x, unsigned char y, unsigned char N);
  void Initial();
  
  private:
  int _rst;
};
#endif