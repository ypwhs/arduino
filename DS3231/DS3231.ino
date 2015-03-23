#include <DS3231.h>
#include <Wire.h>
#include "Timer.h"
Timer ti;
int aDIO = 6, aSCK = 5, aRCK = 4;
unsigned char t[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
unsigned char w[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
DS3231 Clock;
int second, minute, hour, date, month, year, temperature;
long a;

void setup() {
  Wire.begin();
  pinMode(aDIO, OUTPUT); pinMode(aSCK, OUTPUT); pinMode(aRCK, OUTPUT);
  ReadDS3231();
  pinMode(13, OUTPUT);
  ti.every(500, asd);
}
void ReadDS3231()
{
  bool h12, PM;
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  temperature = Clock.getTemperature();
}
void out()
{
  digitalWrite(aRCK, 0);
  digitalWrite(aRCK, 1);
}
void send_1(char a)
{
  int i;
  for (i = 0; i < 8; i++)
  {
    digitalWrite(aSCK, 0);
    digitalWrite(aDIO, a & (0x80));
    a = a << 1;
    digitalWrite(aSCK, 1);
  }
}
void send_2(int a, int i)
{
  send_1(w[i]);
  send_1(t[a % 10]);
  out();
}
void send_2_dot(int a, int i)
{
  send_1(w[i]);
  send_1(t[a % 10] + (1 << 7));
  out();
}
void send_3(int a, int i)
{
  send_2_dot(a % 10, i);
  send_2(a / 10 % 10, i + 1);
}
void display()
{
  send_3(second, 0);
  send_3(minute, 2);
  send_3(hour, 4);
  send_3(temperature, 6);
  send_1(0);
  send_1(0);
  out();
}
int time, s = 1;
void asd()
{
  ReadDS3231();
  time = millis();
  digitalWrite(13, s);
  s = 1 - s;
}
void loop() {
  ti.update();
  display();
}
