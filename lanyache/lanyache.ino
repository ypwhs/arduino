int ea = 11, eb = 10, i1 = 9, i2 = 8, i3 = 7, i4 = 6;
//I1、I2、I3、I4接口，分别用来控制两路直流电机前进、后退
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);// RX, TX

void set(char a,int s)
{
  //设置电机的状态
  if(s!=0){analogWrite(ea,s);analogWrite(eb,s);}
  else {digitalWrite(ea,1);digitalWrite(eb,1);}
  int i;
  for (i = 6; i < 10; i++) {
    digitalWrite(i, a % 2);
    a /= 2;
  }
}

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  int i;
  for (i = 6; i < 12; i++)pinMode(i, OUTPUT);
  digitalWrite(ea, HIGH);
  digitalWrite(eb, HIGH);
  for (i = 6; i < 10; i++)digitalWrite(i, LOW);
}

void loop() {
  char a = mySerial.read();
  if (a == 'F')set(0b1010,0);
  else if (a == 'B')set(0b0101,0);
  else if (a == 'L')set(0b0110,200);
  else if (a == 'R')set(0b1001,200);
  else if (a == 'S')set(0,0);
  delay(10);
}
