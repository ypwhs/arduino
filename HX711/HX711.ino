#include <HX711.h>
HX711 hx(4, 5); //4 SCK    5 DT
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(abs(hx.read()));
}
