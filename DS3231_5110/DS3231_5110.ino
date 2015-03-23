#include <DS3231.h>
#include <Wire.h>
#include <LCD5110_Graph.h>
LCD5110 myGLCD(2, 3, 4, 6, 5);
extern uint8_t SmallFont[];
extern unsigned char TinyFont[];
DS3231 Clock;
bool Century = false, h12 = false, PM;
long s1, s2, ds, dr;
int n = 14, y = 10, r = 1, h = 0, m = 0, s = 0;
int second, minute, hour, date, month, year, temperature;
void setup() {
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  Wire.begin();
  Serial.begin(9600);
}
char a[20];
void ReadDS3231()
{
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  date = Clock.getDate();
  month = Clock.getMonth(Century);
  year = Clock.getYear();
  temperature = Clock.getTemperature();
}
void lhy()
{
  int q[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  ds = 0; dr = 0;
  s1 = hour * 3600 + minute * 60 + second;
  s2 = h * 3600 + m * 60 + s;
  ds = s2 - s1;
  int w = 0, e = 0, b = 0;
  int i, j, k, g;
  for (i = year + 1; i < n; i++)
  {
    if ((i % 400 == 0) || ((i % 4 == 0) && (i % 100 != 0)))
    {
      dr += 366;
    }
    else dr += 365;
  }
  if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))q[1] = 29;
  for (j = month + 1; j <= 12; j++)w += q[j - 1]; w = w + q[month - 1] - date;
  q[1] = 28;
  if ((n % 400 == 0) || ((n % 4 == 0) && (n % 100 != 0)))q[1] = 29;
  for (k = y + 1; k <= 12; k++)
    e += q[k - 1]; e = e + q[y - 1] - r;
  for (g = 1; g < y; g++)
    b += q[g - 1]; b = b + r;
  if (n == year)dr = w - e;
  else dr = dr + b + w;
  ds = dr * 86400 + ds;
}
void shuaxin()
{
  lhy();
  myGLCD.clrScr();
  sprintf(a, "20%02d-%02d-%02d", year, month, date);
  myGLCD.print(a, 0, 0);
  sprintf(a, "%02d:%02d:%02d T=%dC", hour, minute, second,temperature);
  myGLCD.print(a, 0, 8);
  sprintf(a, "%ldsecond", ds);
  myGLCD.print(a, 0, 16);
  sprintf(a, "%ldday", dr);
  myGLCD.print(a, 0, 24);
  sprintf(a, "%02d%02d%02d %02d%02d%02d", n, y, r, h, m, s);
  myGLCD.print(a, 0, 40);
  myGLCD.update();
}

char time[30],time2[30];char rec;
void loop() {

  ReadDS3231();
  shuaxin();
  rec=Serial.read();
  if ( rec== 'a')
  {
    Serial.print("a");
    delay(50);
    Serial.readBytesUntil('a', time, 30);
    Serial.println(time);
    sscanf(time, "20%d-%d-%d %d:%d:%d", &year, &month, &date, &hour, &minute, &second);
    Clock.setSecond(second);
    Clock.setMinute(minute);
    Clock.setHour(hour);
    Clock.setDate(date);
    Clock.setMonth(month);
    Clock.setYear(year);
  }
    if (rec == 'b')
  {
    Serial.print("b");
    delay(50);
    Serial.readBytesUntil('b', time2, 30);
    Serial.println(time2);
    sscanf(time2, "20%d-%d-%d %d:%d:%d", &n, &y, &r, &h, &m, &s);
  }
  
  delay(100);
}
