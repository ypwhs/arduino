#include <DS3231.h>
#include <Wire.h>
#include <U8glib.h>
#include <DHT.h>
#include<EEPROM.h>
#include<avr/sleep.h>
DHT dht;
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);
extern uint8_t SmallFont[];
int q[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
extern unsigned char TinyFont[];
char zidingyi[60] = "Copyright lxm. All rights reserved.";
DS3231 Clock;
bool Century = false, h12 = false, PM;
long ds, dd;
int n = 14, y = 10, r = 1, h = 0, m = 0, s = 0;
int second, minute, hour, date, month, year, temperature;
float humidity, temp;

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
void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x12);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}
int shangyimiao=0;
void shuaxin()
{
  if(second!=shangyimiao){
  int wz = 0, hei = 9,l=1;
  lhy();
  u8g_prepare();
  u8g.firstPage();
  do {
    wz = -1;
    u8g.drawFrame(0,0,127,19);
    sprintf(a, "20%02d-%02d-%02d %02d:%02d:%02d", year, month, date, hour, minute, second);
    u8g.drawStr( l+6, wz+1, a); wz += hei;
    sprintf(a, "Wendu=%dC Shidu=%d%%",  (int)temp, (int) humidity);
    u8g.drawStr( l+6, wz, a); wz += hei;
    sprintf(a, "To20%02d-%02d-%02d %02d:%02d:%02d", n, y, r, h, m, s);
    u8g.drawStr( l, wz+1, a); wz += hei;
    sprintf(a, "Left:%lddays", dd);
    u8g.drawStr( l, wz, a); wz += hei;
    sprintf(a, "%ldseconds", ds);
    u8g.drawStr( l, wz, a); wz += hei;
    if (strlen(zidingyi) < 22) {
      u8g.drawStr( l, wz, zidingyi); wz += hei;
    }
    else {
      char tmp = zidingyi[21]; zidingyi[21] = 0;
      u8g.drawStr( l, wz, zidingyi); wz += hei;
      zidingyi[21] = tmp;
      u8g.drawStr( l, wz, zidingyi + 21); wz += hei;
    }
  }
  while ( u8g.nextPage() );
  shangyimiao=second;
  }
}
int delaytime, nowtime, lasttime;
char time[45], time2[45]; char rec;

void setup() {
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
    u8g.setHiColorByRGB(255, 255, 255);
  }
  Wire.begin();
  Serial.begin(9600);
  dht.setup(7);
  int i;
  if(EEPROM.read(0)!=255){
    for(i=0;i<=EEPROM.read(0)&&i<45;i++)
    time2[i]=EEPROM.read(i+1);
    sscanf(time2, "20%d-%d-%d %d:%d:%d", &n, &y, &r, &h, &m, &s);
  }
  if(EEPROM.read(100)!=255){
    for(i=0;i<=EEPROM.read(100)&&i<60;i++)
    zidingyi[i]=EEPROM.read(101+i);
  }
  pinMode(2,INPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);sleep_enable();
  Clock.enableOscillator(true,false,0);
  attachInterrupt(0, test, CHANGE);
}
void test()
{
  //digitalWrite(13,i);
  //i=1-i;
  //sleep_disable(); 
}


void loop()
{
  nowtime = millis();

  ReadDS3231();
  shuaxin();
  
  if (nowtime - lasttime >= delaytime) {
    delaytime = dht.getMinimumSamplingPeriod();
    lasttime = nowtime;
    humidity = dht.getHumidity();
    temp = dht.getTemperature();
  }
  if(Serial.available())
  {
  rec = Serial.read();
  if ( rec == 'a')
  {
    Serial.print("a");
    delay(50);
    Serial.readBytesUntil('a', time, 45);
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
    Serial.readBytesUntil('b', time2, 45);
    Serial.println(time2);
    int i;
    EEPROM.write(0,strlen(time2));
    for(i=0;i<=strlen(time2)&&i<45;i++)EEPROM.write(i+1,time2[i]);
    sscanf(time2, "20%d-%d-%d %d:%d:%d", &n, &y, &r, &h, &m, &s);
  }
  if (rec == '*')
  {
    Serial.print("*");
    delay(50);
    Serial.readBytesUntil('*', zidingyi, 60);
        int i;
    EEPROM.write(100,strlen(zidingyi));
    for(i=0;i<=strlen(zidingyi)&&i<=60;i++)
    {EEPROM.write(101+i,zidingyi[i]);}
    Serial.println(zidingyi);
  }
  }
 sleep_cpu();
}
