#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Servo.h>

#define YUZHI 500

LiquidCrystal_I2C lcd(0x27, 20, 4);
DS3231 Clock;
DHT dht;
Servo myservo;

void setup()
{
  myservo.attach(3);
  myservo.write(90);
  Serial.begin(115200);
  dht.setup(2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(A0, INPUT);

}
int second, minute, hour, date, month, year;
bool h12;
bool PM;
bool Century = false;
void refreshTime() {
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour = Clock.getHour(h12, PM);
  date = Clock.getDate();
  month = Clock.getMonth(Century);
  year = Clock.getYear();
}
char time[45], a[45];
float humidity = 0, temperature = 0;
long last, now;


void loop()
{
  refreshTime();

  if (Serial.available() > 0)
  {
    char rec = Serial.read();
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
  }


  sprintf(a, "20%02d-%02d-%02d %02d:%02d:%02d", year, month, date, hour, minute, second);
  lcd.setCursor(0, 0);
  lcd.print(a);

  int sensor = analogRead(A0);
  sprintf(a, "Sensor:%4d ", sensor);
  lcd.setCursor(0, 1);
  lcd.print(a);
  if (sensor < YUZHI) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }

  now = millis();
  if (now - last > dht.getMinimumSamplingPeriod()) {

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    lcd.setCursor(0, 2);
    lcd.print(dht.getStatusString());
    lcd.print(" C: ");
    lcd.print(temperature);
    lcd.print(" H: ");
    lcd.print(humidity);
    last = now;
  }

  if ( ((hour == 11) && (minute > 30) && (minute < 35)) |
    ((hour == 17) && (minute < 5)) )
  {
    if (sensor > YUZHI) {
      myservo.write(60);
      delay(500);
      myservo.write(90);
      delay(500);
    }
  }

  delay(50);
}
