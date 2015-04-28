#include <IRremote.h>
IRsend irsend;
void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  irsend.sendNEC(0xFFA25D, 32);
  delay(1000);
}
