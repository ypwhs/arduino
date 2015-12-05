#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WANG YU");
  lcd.setCursor(2, 1);
  lcd.print("HAHA HAHA");
  lcd.setCursor(4, 2);
  lcd.print("1234567890");
  lcd.setCursor(6, 3);
  lcd.print("ASDFGHJKL");
}
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    char a = Serial.read();
    lcd.setCursor(i, 0);      //x,y
    lcd.print(a);
    i++;
    if(i>=20)i=0;
  }
}
