#include <HX711.h>
HX711 hx(4, 5);
//4 SCK    5 DT
void setup() {
  pinMode(13,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  Serial.begin(9600);
}

void loop()
{
  double sum = 0;
  Serial.println(abs(hx.read()));
}
