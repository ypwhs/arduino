#include <DS3231.h>
#include <Wire.h>
#include <LCD5110_Graph.h>
#include <DHT.h>
DHT dht;
LCD5110 myGLCD(2, 3, 4, 6, 5);
extern uint8_t SmallFont[];
int q[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
extern unsigned char TinyFont[];
char zidingyi[30] = "BY:lhy";
DS3231 Clock;
bool Century = false, h12 = false, PM;
long ds, dd;
int n = 14, y = 10, r = 1, h = 0, m = 0, s = 0;
int second, minute, hour, date, month, year, temperature;
float humidity, temp;
void setup() {
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  Wire.begin();
  Serial.begin(9600);
  dht.setup(7);
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
int runnian(int i)
{
  if ((i % 400 == 0) || ((i % 4 == 0) && (i % 100 != 0)))return 1;
  else return 0;
}
void lhy()
{
  long s1, s2, d1 = 0, d2 = 0;
  ds = 0; dd = 0;
  s1 = hour * 3600 + minute * 60 + second;
  s2 = h * 3600 + m * 60 + s;
  ds = s2 - s1;
  int i, j;
  for (i = year + 1; i < n; i++)
  {
    if (runnian(i))
    {
      dd += 366;
    }
    else dd += 365;
  }
  if (runnian(year))q[1] = 29; else q[1] = 28;
  for (j = 1; j <= month; j++)
    d1 += q[j - 1]; d1 += date;

  if (runnian(n))q[1] = 29; else q[1] = 28;
  for (j = 1; j <= y; j++)
    d2 += q[j - 1]; d2 += r;
  dd += (d2 - d1);
  ds = dd * 86400 + ds;
}
void shuaxin()
{
  lhy();
  myGLCD.clrScr();
  sprintf(a, "20%02d-%02d-%02d", year, month, date);
  myGLCD.print(a, 0, 0);
  sprintf(a, "%02d:%02d:%02d T=%dC", hour, minute, second, (int)temp);
  myGLCD.print(a, 0, 8);
  sprintf(a, "%ldsecond", ds);
  myGLCD.print(a, 0, 16);
  sprintf(a, "%ldday H=%d%%", dd, (int) humidity);
  myGLCD.print(a, 0, 24);
  sprintf(a, "%02d%02d%02d %02d%02d%02d", n, y, r, h, m, s);
  myGLCD.print(zidingyi, 0, 32);
  myGLCD.print(a, 0, 41);
  myGLCD.update();
}
int delaytime, nowtime, lasttime;
char time[30], time2[30]; char rec;
void loop() {
  nowtime = millis();
  ReadDS3231();
  shuaxin();
  if (nowtime - lasttime >= delaytime) {
    delaytime = dht.getMinimumSamplingPeriod();
    lasttime = nowtime;
    humidity = dht.getHumidity();
    temp = dht.getTemperature();
  }
  rec = Serial.read();
  if ( rec == 'a')
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
  if (rec == '*')
  {
    Serial.print("*");
    delay(50);
    Serial.readBytesUntil('*', zidingyi, 30);
    Serial.println(zidingyi);
  }

  delay(100);
}
